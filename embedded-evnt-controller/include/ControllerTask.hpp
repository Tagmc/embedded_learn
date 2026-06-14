#pragma once

#include "BlockingEventQueue.hpp"
#include "Event.hpp"
#include "TaskStats.hpp"
#include "VehicleController.hpp"

#include <atomic>
#include <chrono>
#include <cstddef>
#include <thread>

template <std::size_t QueueCapacity>
class ControllerTask {
public:
    ControllerTask(
        BlockingEventQueue<QueueCapacity>& eventQueue,
        VehicleController& controller
    )
        : eventQueue_(eventQueue),
          controller_(controller) {}

    void start() {
        running_.store(true);

        thread_ = std::thread([this]() {
            run();
        });
    }

    void stop() {
        running_.store(false);
        eventQueue_.stop();

        if (thread_.joinable()) {
            thread_.join();
        }
    }

    ~ControllerTask() {
        stop();
    }

    ControllerTask(const ControllerTask&) = delete;
    ControllerTask& operator=(const ControllerTask&) = delete;

    [[nodiscard]] const TaskStats& stats() const {
        return stats_;
    }

private:
    void run() {
        while (running_.load()) {
            Event event{};

            const auto status = eventQueue_.waitAndPop(
                event,
                std::chrono::milliseconds(200)
            );

            switch (status) {
                case BlockingQueuePopStatus::Ok:
                    controller_.handle(event);
                    ++stats_.eventsHandled;
                    break;

                case BlockingQueuePopStatus::Timeout:
                    ++stats_.timeouts;
                    break;

                case BlockingQueuePopStatus::Stopped:
                    return;
            }
        }
    }

    BlockingEventQueue<QueueCapacity>& eventQueue_;
    VehicleController& controller_;
    std::atomic<bool> running_ = false;
    std::thread thread_;
    TaskStats stats_{};
};