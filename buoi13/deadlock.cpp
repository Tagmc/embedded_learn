#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
std::mutex mutexA;
std::mutex mutexB;

void thread1() {

    std::lock_guard<std::mutex> lockA(mutexA);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> lockB(mutexB);
    std::cout << "Thread1 done" << '\n';
}
void thread2() {
    std::lock_guard<std::mutex> lockB(mutexB);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::lock_guard<std::mutex> lockA(mutexA);

    std::cout << "thread2 done\n";
}

int main() {
    std::thread first(thread1);
    std::thread second(thread2);
    first.join();
    second.join();
}