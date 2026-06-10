#include <chrono>
#include <iostream>
#include <vector>

template <typename Func>
long long measureMicroseconds(Func func) {
    const auto start = std::chrono::steady_clock::now();
    func();
    const auto end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main() {
    std::vector<int> original(10'000'000, 1);

    long long copySize = 0;

    const long long copyTime = measureMicroseconds([&]() {
        std::vector<int> copy = original;
        copySize = static_cast<long long>(copy.size());
    });

    std::vector<int> movable(10'000'000, 1);

    long long moveSize = 0;

    const long long moveTime = measureMicroseconds([&]() {
        std::vector<int> moved = std::move(movable);
        moveSize = static_cast<long long>(moved.size());
    });

    std::cout << "copySize = " << copySize << ", copyTime = " << copyTime << "us\n";
    std::cout << "moveSize = " << moveSize << ", moveTime = " << moveTime << "us\n";

    return 0;
}