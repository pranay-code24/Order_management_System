#pragma once

#include <string>
#include <chrono>

enum class RequestType {
    Unknown = 0, New = 1, Modify = 2, Cancel = 3
};

enum class ResponseType {
    Unknown = 0, Accept = 1, Reject = 2
};

struct Logon {
    std::string username;
    std::string password;
};

struct Logout {
    std::string username;
};

struct OrderRequest {
    int m_symbolId;
    double m_price;
    uint64_t m_qty;
    char m_side;
    uint64_t m_orderId;
    RequestType m_type;
    std::chrono::steady_clock::time_point m_receivedTime;
};

struct OrderResponse {
    uint64_t m_orderId;
    ResponseType m_responseType;
};
