#pragma once

#include "Event.hpp"
#include "RingBuffer.hpp"

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <optional>

enum class BlockingQueuePostStatus {
    Ok,
    Full,
    Stopped
};

enum class BlockingQueuePopStatus {
    Ok,
    Timeout,
    Stopped
};

template <std::size_t Capacity>
class BlockingEventQueue {
public:
    static_assert(Capacity > 0, "BlockingEventQueue capacity must be greater than zero");

    [[nodiscard]] BlockingQueuePostStatus post(const Event& event) {
        {
            std::lock_guard<std::mutex> lock(mutex_);

            if (stopped_) {
                return BlockingQueuePostStatus::Stopped;
            }

            if (!buffer_.push(event)) {
                return BlockingQueuePostStatus::Full;
            }
        }

        notEmpty_.notify_one();
        return BlockingQueuePostStatus::Ok;
    }

    [[nodiscard]] BlockingQueuePopStatus waitAndPop(
        Event& out,
        std::chrono::milliseconds timeout
    ) {
        std::unique_lock<std::mutex> lock(mutex_);

        const bool ready = notEmpty_.wait_for(
            lock,
            timeout,
            [this]() {
                return stopped_ || !buffer_.empty();
            }
        );

        if (!ready) {
            return BlockingQueuePopStatus::Timeout;
        }

        if (stopped_ && buffer_.empty()) {
            return BlockingQueuePopStatus::Stopped;
        }

        auto event = buffer_.pop();

        if (!event.has_value()) {
            return stopped_
                ? BlockingQueuePopStatus::Stopped
                : BlockingQueuePopStatus::Timeout;
        }

        out = *event;
        return BlockingQueuePopStatus::Ok;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stopped_ = true;
        }

        notEmpty_.notify_all();
    }

    [[nodiscard]] bool stopped() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return stopped_;
    }

    [[nodiscard]] std::size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return buffer_.size();
    }

    [[nodiscard]] bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return buffer_.empty();
    }

    [[nodiscard]] bool full() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return buffer_.full();
    }

    [[nodiscard]] constexpr std::size_t capacity() const {
        return Capacity;
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable notEmpty_;
    RingBuffer<Event, Capacity> buffer_;
    bool stopped_ = false;
};