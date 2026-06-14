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
class ButtonTask {
public:
    explicit ButtonTask(BlockingEventQueue<QueueCapacity>& eventQueue)
        : eventQueue_(eventQueue) {}

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

    ~ButtonTask() {
        stop();
    }

    ButtonTask(const ButtonTask&) = delete;
    ButtonTask& operator=(const ButtonTask&) = delete;

    [[nodiscard]] const TaskStats& stats() const {
        return stats_;
    }

private:
    void run() {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));

        if (!running_.load()) {
            return;
        }

        post(Event{EventType::ButtonPressed, 150, 0});

        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        if (!running_.load()) {
            return;
        }

        post(Event{EventType::ButtonPressed, 400, 0});
    }

    void post(const Event& event) {
        const auto status = eventQueue_.post(event);

        if (status == BlockingQueuePostStatus::Ok) {
            ++stats_.eventsPosted;
        } else {
            ++stats_.postFailures;
        }
    }

    BlockingEventQueue<QueueCapacity>& eventQueue_;
    std::atomic<bool> running_ = false;
    std::thread thread_;
    TaskStats stats_{};
};