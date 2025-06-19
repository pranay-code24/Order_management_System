#pragma once

#include "Types.h"
#include <unordered_map>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <fstream>

class OrderManagement {
public:
    OrderManagement(int throttleLimitPerSecond,
                    std::chrono::system_clock::time_point marketStart,
                    std::chrono::system_clock::time_point marketEnd);

    ~OrderManagement();

    void onData(OrderRequest&& request);
    void onData(OrderResponse&& response);

    void send(const OrderRequest& request);
    void sendLogon();
    void sendLogout();

private:
    bool withinMarketTime();
    void orderSenderThread();
    void marketTimeWatcher();
    void persistResponse(uint64_t orderId, ResponseType type, long latencyMicros);

    std::mutex mtx;
    std::condition_variable cv;

    int throttleLimit;
    std::atomic<int> sentCount;
    std::atomic<bool> marketOpen;
    std::atomic<bool> running;

    std::chrono::system_clock::time_point marketStart;
    std::chrono::system_clock::time_point marketEnd;

    std::queue<OrderRequest> orderQueue;
    std::unordered_map<uint64_t, OrderRequest> queuedOrders;
    std::unordered_map<uint64_t, std::chrono::steady_clock::time_point> sentOrders;

    std::thread senderThread;
    std::thread marketThread;
};
