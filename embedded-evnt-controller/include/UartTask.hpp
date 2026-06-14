#pragma once

#include "BlockingEventQueue.hpp"
#include "CommandParser.hpp"
#include "Event.hpp"
#include "TaskStats.hpp"

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <thread>

template <std::size_t QueueCapacity>
class UartTask {
public:
    UartTask(BlockingEventQueue<QueueCapacity>& eventQueue)
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

    ~UartTask() {
        stop();
    }

    UartTask(const UartTask&) = delete;
    UartTask& operator=(const UartTask&) = delete;

    [[nodiscard]] const TaskStats& stats() const {
        return stats_;
    }

private:
    void run() {
        sendCommandText("LED ON\n", 100);
        sendCommandText("LED TOGGLE\n", 200);
        sendCommandText("HELLO\n", 300);
        sendCommandText("FAULT\n", 400);
        sendCommandText("LED OFF\n", 500);
        sendCommandText("RESET\n", 600);
    }

    void sendCommandText(const char* text, std::uint32_t timestampMs) {
        for (std::size_t i = 0; text[i] != '\0'; ++i) {
            if (!running_.load()) {
                return;
            }

            const auto result = parser_.feed(
                static_cast<std::uint8_t>(text[i])
            );

            if (result.status == CommandParseStatus::CommandReady) {
                if (result.command.has_value()) {
                    postCommand(*result.command, timestampMs);
                }
            } else if (
                result.status == CommandParseStatus::InvalidCommand ||
                result.status == CommandParseStatus::BufferOverflow
            ) {
                ++stats_.postFailures;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void postCommand(CommandId command, std::uint32_t timestampMs) {
        const Event event{
            EventType::UartCommandReceived,
            timestampMs,
            static_cast<std::uint32_t>(command)
        };

        const auto status = eventQueue_.post(event);

        if (status == BlockingQueuePostStatus::Ok) {
            ++stats_.eventsPosted;
        } else {
            ++stats_.postFailures;
        }
    }

    BlockingEventQueue<QueueCapacity>& eventQueue_;
    CommandParser parser_;
    std::atomic<bool> running_ = false;
    std::thread thread_;
    TaskStats stats_{};
};