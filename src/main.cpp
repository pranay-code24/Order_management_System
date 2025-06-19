#include "../include/OrderManagement.h"
#include <thread>

int main() {
    using clock = std::chrono::system_clock;

    auto now = clock::now();
    auto start = now + std::chrono::seconds(2);
    auto end = now + std::chrono::seconds(20);

    OrderManagement om(2, start, end); 

    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (int i = 1; i <= 5; ++i) {
        uint64_t qty = 10 + i;
        uint64_t orderId = static_cast<uint64_t>(i);
        OrderRequest req = {100, 150.5 + i, qty, 'B', orderId, RequestType::New, {}};
        om.onData(std::move(req));
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    // Modify and cancel one
    OrderRequest mod = {100, 199.9, 99, 'B', 3, RequestType::Modify, {}};
    om.onData(std::move(mod));

    OrderRequest cancel = {100, 0, 0, 'B', 4, RequestType::Cancel, {}};
    om.onData(std::move(cancel));

    std::this_thread::sleep_for(std::chrono::seconds(25));
}
