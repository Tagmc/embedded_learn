#pragma once
#include "Event.hpp"
#include "Led.hpp"
#include <cstdint>

class SimpleController
{
public:
    explicit SimpleController(Led &statusLed);
    void handle(const Event &event);
    [[nodiscard]] std::uint32_t handledEvents() const;
    [[nodiscard]] std::uint32_t timerTicks() const;
    [[nodiscard]] std::uint32_t sensorErrors() const;

private:
    void handleButtonPressed(const Event &event);
    void handleButtonReleased(const Event &event);
    void handleTimerTick(const Event &event);
    void handleUartCommand(const Event &event);
    void handleSensorError(const Event &event);
    void handleFaultDetected(const Event &event);
    Led &statusLed_;
    std::uint32_t handledEvents_ = 0;
    std::uint32_t timerTicks_ = 0;
    std::uint32_t sensorErrors_ = 0;
};