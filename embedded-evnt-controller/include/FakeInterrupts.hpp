#pragma once

#include "Event.hpp"
#include "EventQueue.hpp"

#include <cstddef>
#include <cstdint>

template <std::size_t QueueCapacity>
class FakeInterrupts {
public:
    FakeInterrupts(
        EventQueue<QueueCapacity>& eventQueue,
        std::uint32_t& droppedEvents
    )
        : eventQueue_(eventQueue),
          droppedEvents_(droppedEvents) {}

    void onButtonPressed(std::uint32_t timestampMs) {
        postOrCount(Event{EventType::ButtonPressed, timestampMs, 0});
    }

    void onButtonReleased(std::uint32_t timestampMs) {
        postOrCount(Event{EventType::ButtonReleased, timestampMs, 0});
    }

    void onTimerTick(std::uint32_t timestampMs, std::uint32_t tickCount) {
        postOrCount(Event{EventType::TimerTick, timestampMs, tickCount});
    }

    void onUartCommand(std::uint32_t timestampMs, std::uint32_t commandId) {
        postOrCount(Event{EventType::UartCommandRecieved, timestampMs, commandId});
    }

    void onSensorError(std::uint32_t timestampMs, std::uint32_t errorCode) {
        postOrCount(Event{EventType::SensorError, timestampMs, errorCode});
    }

    void onFaultDetected(std::uint32_t timestampMs, std::uint32_t faultCode) {
        postOrCount(Event{EventType::FaultDetected, timestampMs, faultCode});
    }

private:
    void postOrCount(const Event& event) {
        const auto status = eventQueue_.post(event);

        if (status == EventQueueStatus::Full) {
            ++droppedEvents_;
        }
    }

    EventQueue<QueueCapacity>& eventQueue_;
    std::uint32_t& droppedEvents_;
};