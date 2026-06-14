#pragma once

#include "BlockingEventQueue.hpp"
#include "Event.hpp"
#include "TaskStats.hpp"

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <thread>

template <std::size_t QueueCapacity>
class TimerTask {
public:
    TimerTask(
        BlockingEventQueue<QueueCapacity>& eventQueue,
        std::chrono::milliseconds period
    )
        : eventQueue_(eventQueue),
          period_(period) {}

    void start() {
        running_.store(true);

        thread_ = std::thread([this]() {
            run();
        });
    }

    void stop() {
        running_.store(false);

        if (thread_.joinable()) {
            thread_.join();
        }
    }

    ~TimerTask() {
        stop();
    }

    TimerTask(const TimerTask&) = delete;
    TimerTask& operator=(const TimerTask&) = delete;

    [[nodiscard]] const TaskStats& stats() const {
        return stats_;
    }

private:
    void run() {
        std::uint32_t tick = 0;
        std::uint32_t timestampMs = 0;

        while (running_.load()) {
            std::this_thread::sleep_for(period_);

            if (!running_.load()) {
                break;
            }

            ++tick;
            timestampMs += static_cast<std::uint32_t>(period_.count());

            const Event event{
                EventType::TimerTick,
                timestampMs,
                tick
            };

            const auto status = eventQueue_.post(event);

            if (status == BlockingQueuePostStatus::Ok) {
                ++stats_.eventsPosted;
            } else {
                ++stats_.postFailures;
            }
        }
    }

    BlockingEventQueue<QueueCapacity>& eventQueue_;
    std::chrono::milliseconds period_;
    std::atomic<bool> running_ = false;
    std::thread thread_;
    TaskStats stats_{};
};