#pragma once

#include <cstdint>

enum class EventType
{
    ButtonPressed,
    ButtonReleased,
    TimerTick,
    UartCommandRecieved,
    SensorError,
    FaultDetected
};

struct Event {
    EventType type = EventType::TimerTick;
    std::uint32_t timestampMs = 0;
    std::uint32_t data = 0;
};