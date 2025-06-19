#include <iostream>
#include <fstream>
#include <random>
#include <chrono>

int main() {
    std::ofstream out("responses.csv", std::ios::out | std::ios::trunc); // overwrite

    if (!out.is_open()) {
        std::cerr << "Failed to open responses.csv\n";
        return 1;
    }

    std::default_random_engine generator(
        static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count())
    );

    std::uniform_int_distribution<int> responseTypeDist(1, 2);  // 1 = Accept, 2 = Reject
    std::uniform_int_distribution<int> latencyDist(100, 250);   // in microseconds

    const int numEntries = 20;

    for (int i = 1; i <= numEntries; ++i) {
        int responseType = responseTypeDist(generator);
        int latency = latencyDist(generator);

        out << i << "," << responseType << "," << latency << "us\n";
    }

    out.close();
    std::cout << "✅ Generated mock responses.csv with " << numEntries << " entries.\n";
    return 0;
}
