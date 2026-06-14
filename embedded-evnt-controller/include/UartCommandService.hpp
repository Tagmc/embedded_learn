#pragma once

#include "Command.hpp"
#include "CommandParser.hpp"
#include "Event.hpp"
#include "EventQueue.hpp"
#include "RingBuffer.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>

enum class UartReceiveStatus {
    Ok,
    RxBufferFull
};

struct UartCommandServiceStats {
    std::uint32_t receivedBytes = 0;
    std::uint32_t droppedBytes = 0;
    std::uint32_t parserErrors = 0;
    std::uint32_t commandsPosted = 0;
    std::uint32_t eventQueueFull = 0;
};

template <std::size_t RxCapacity, std::size_t EventQueueCapacity>
class UartCommandService {
public:
    explicit UartCommandService(EventQueue<EventQueueCapacity>& eventQueue)
        : eventQueue_(eventQueue) {}

    [[nodiscard]] UartReceiveStatus receiveByte(std::uint8_t byte) {
        ++stats_.receivedBytes;

        if (!rxBuffer_.push(byte)) {
            ++stats_.droppedBytes;
            return UartReceiveStatus::RxBufferFull;
        }

        return UartReceiveStatus::Ok;
    }

    std::size_t processReceivedBytes(std::uint32_t timestampMs) {
        std::size_t processedBytes = 0;

        while (!rxBuffer_.empty()) {
            auto byte = rxBuffer_.pop();

            if (!byte.has_value()) {
                break;
            }

            handleByte(*byte, timestampMs);
            ++processedBytes;
        }

        return processedBytes;
    }

    [[nodiscard]] bool hasPendingBytes() const {
        return !rxBuffer_.empty();
    }

    [[nodiscard]] std::size_t pendingBytes() const {
        return rxBuffer_.size();
    }

    [[nodiscard]] const UartCommandServiceStats& stats() const {
        return stats_;
    }

    void resetStats() {
        stats_ = UartCommandServiceStats{};
    }

private:
    void handleByte(std::uint8_t byte, std::uint32_t timestampMs) {
        const auto result = parser_.feed(byte);

        switch (result.status) {
            case CommandParseStatus::InProgress:
                break;

            case CommandParseStatus::CommandReady:
                handleCommandReady(result.command, timestampMs);
                break;

            case CommandParseStatus::InvalidCommand:
            case CommandParseStatus::BufferOverflow:
                ++stats_.parserErrors;
                break;

            case CommandParseStatus::EmptyLine:
                break;
        }
    }

    void handleCommandReady(
        const std::optional<CommandId>& command,
        std::uint32_t timestampMs
    ) {
        if (!command.has_value()) {
            ++stats_.parserErrors;
            return;
        }

        const Event event{
            EventType::UartCommandReceived,
            timestampMs,
            static_cast<std::uint32_t>(*command)
        };

        const auto status = eventQueue_.post(event);

        if (status == EventQueueStatus::Full) {
            ++stats_.eventQueueFull;
            return;
        }

        ++stats_.commandsPosted;
    }

    RingBuffer<std::uint8_t, RxCapacity> rxBuffer_;
    CommandParser parser_;
    EventQueue<EventQueueCapacity>& eventQueue_;
    UartCommandServiceStats stats_{};
};