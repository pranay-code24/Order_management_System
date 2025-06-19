# Order Management System

This project is a multithreaded **Order Management System** implemented in C++.

---

## 🚀 Features

- ⏰ **Market Time Handling**
  - System sends a `Logon` message at market open and a `Logout` at market close.
  - Orders outside the allowed time window are automatically rejected.

- 📉 **Throttling**
  - Limits the number of orders sent to exchange per second (`X` orders/sec).
  - Excess orders are queued and dispatched in the next eligible window.

- 🧾 **Order Types Supported**
  - `New`: Queued or sent based on throttle.
  - `Modify`: Updates an existing queued order (price/quantity).
  - `Cancel`: Removes an existing queued order.

- 🧪 **Latency Logging**
  - Measures round-trip latency of each order and writes it to `responses.csv`.

- 🔒 **Thread-Safe**
  - Clean concurrent handling using `std::mutex`, `std::thread`, and `std::queue`.

---

## 🧱 Tech Stack

- **Language**: C++17
- **Concurrency**: `std::thread`, `std::mutex`, `std::atomic`
- **Build**: Makefile (or manual compilation)
- **Platform**: Cross-platform (tested on macOS)

---

## 📂 Folder Structure

```

OrderManagementSystem/
├── include/
│   ├── Types.h
│   └── OrderManagement.h
├── src/
│   ├── OrderManagement.cpp
│   └── main.cpp
├── responses.csv
└── Makefile

```

---

## 🛠️ Build & Run

### Compile
```bash
make run
```

### Clean
```bash
make clean
```

> Or manually compile:

```bash
clang++ -std=c++17 -pthread -Iinclude src/*.cpp -o order_system
```

---

## 📈 Output

On running, the console will display:

* Logon message
* Order processing results
* Logout message

Additionally, `responses.csv` is created to store order ID, response type, and latency.

---

## 📌 Assumptions

* Order types (`New`, `Modify`, `Cancel`) are properly tagged.
* Exchange send/receive is simulated (no real TCP/socket communication).
* Time window is passed at initialization.
* Upstream systems are simulated via the `main.cpp`.

---

## ✍️ Author

**Pranay Gumashta**
pranaygumashta@gmail.com