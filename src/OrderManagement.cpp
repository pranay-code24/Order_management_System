#include "../include/OrderManagement.h"
#include <iostream>

OrderManagement::OrderManagement(int limit, std::chrono::system_clock::time_point start, std::chrono::system_clock::time_point end) : throttleLimit(limit), marketStart(start), marketEnd(end), sentCount(0), marketOpen(false), running(true) {
    senderThread = std::thread(&OrderManagement::orderSenderThread, this);
    marketThread = std::thread(&OrderManagement::marketTimeWatcher, this);
}

OrderManagement::~OrderManagement() {
    running = false;
    if (senderThread.joinable()) senderThread.join();
    if (marketThread.joinable()) marketThread.join();
}

bool OrderManagement::withinMarketTime() {
    auto now = std::chrono::system_clock::now();
    return now >= marketStart && now <= marketEnd;
}

void OrderManagement::send(const OrderRequest& request) {
    std::cout << "[Exchange] Sent Order: " << request.m_orderId << std::endl;
}

void OrderManagement::sendLogon() {
    std::cout << "[Exchange] Sent Logon\n";
}

void OrderManagement::sendLogout() {
    std::cout << "[Exchange] Sent Logout\n";
}

void OrderManagement::persistResponse(uint64_t id, ResponseType type, long latency) {
    std::ofstream file("responses.csv", std::ios::app);
    file << id << "," << static_cast<int>(type) << "," << latency << "us\n";
    file.close();
}

void OrderManagement::onData(OrderRequest&& request) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!withinMarketTime()) {
        std::cout << "[Rejected] Market Closed. Order: " << request.m_orderId << "\n";
        return;
    }

    request.m_receivedTime = std::chrono::steady_clock::now();

    if (request.m_type == RequestType::New) {
        if (sentCount < throttleLimit) {
            send(request);
            sentOrders[request.m_orderId] = std::chrono::steady_clock::now();
            ++sentCount;
        } else {
            orderQueue.push(request);
            queuedOrders[request.m_orderId] = request;
        }
    } else if (request.m_type == RequestType::Modify) {
        if (queuedOrders.count(request.m_orderId)) {
            queuedOrders[request.m_orderId].m_price = request.m_price;
            queuedOrders[request.m_orderId].m_qty = request.m_qty;
        }
    } else if (request.m_type == RequestType::Cancel) {
        if (queuedOrders.count(request.m_orderId)) {
            queuedOrders.erase(request.m_orderId);
        }
    }
}

void OrderManagement::onData(OrderResponse&& response) {
    std::lock_guard<std::mutex> lock(mtx);

    auto now = std::chrono::steady_clock::now();
    if (sentOrders.count(response.m_orderId)) {
        auto sentTime = sentOrders[response.m_orderId];
        long latency = std::chrono::duration_cast<std::chrono::microseconds>(now - sentTime).count();
        persistResponse(response.m_orderId, response.m_responseType, latency);
        sentOrders.erase(response.m_orderId);
    }
}

void OrderManagement::orderSenderThread() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        sentCount = 0;

        std::lock_guard<std::mutex> lock(mtx);
        while (!orderQueue.empty() && sentCount < throttleLimit) {
            OrderRequest req = orderQueue.front();
            orderQueue.pop();
            queuedOrders.erase(req.m_orderId);

            send(req);
            sentOrders[req.m_orderId] = std::chrono::steady_clock::now();
            ++sentCount;
        }
    }
}

void OrderManagement::marketTimeWatcher() {
    while (running) {
        auto now = std::chrono::system_clock::now();
        if (!marketOpen && now >= marketStart && now < marketEnd) {
            sendLogon();
            marketOpen = true;
        } else if (marketOpen && now >= marketEnd) {
            sendLogout();
            marketOpen = false;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
