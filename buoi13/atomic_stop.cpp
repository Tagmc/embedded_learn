#include <atomic>
#include <thread>
#include <iostream>
#include <chrono>

class Worker {
    public:
        void start() {
            thread_ = std::thread([this] {
                run();
            });
        }
        void stop() {
            stop_.store(true);
            if (thread_.joinable()) {
                thread_.join();
            }
        }
        ~Worker() {
            stop();
        }

    private:
        void run() {
            while (!stop_.load()) {
                std::cout << "working..." << '\n';
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            std::cout << "stopped";
        }
        std::atomic<bool> stop_ = false;
        std::thread thread_;
};


int main() {
    Worker worker;
    worker.start();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    worker.stop();
    return 0;
}