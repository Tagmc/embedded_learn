#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <optional>

template <typename T>
class ThreadSafeQueue
{
public:
    void push(T value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(value));
    }
    std::optional<T> tryPop()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            return std::nullopt;
        }
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

private:
    mutable std::mutex mutex_;
    std::queue<int> queue_;
};

int main()
{
    ThreadSafeQueue<int> queue;
    std::thread producer([&queue]()
                         { for (int i = 0; i < 10; ++i) {
            queue.push(i);
        } });
    std::thread consumer([&queue]()
                         { for (int i = 0; i < 10; ++i) {
                            while (true) {
                                std::optional<int> value = queue.tryPop();
                                if (value.has_value()) {
                                    std::cout << "value= " << *value << '\n';
                                    break;
                                }
                            }
                         } });
    producer.join();
    consumer.join();
}