#pragma once

#include "Command.hpp"
#include "Event.hpp"
#include "Led.hpp"
#include "SystemState.hpp"

#include <cstdint>

class VehicleController
{
public:
    explicit VehicleController(Led &statusLed);
    [[nodiscard]] bool initialize();
    void handle(const Event &event);
    [[nodiscard]] SystemState state() const;

    [[nodiscard]] std::uint32_t handledEvents() const;

    [[nodiscard]] std::uint32_t ignoredEvents() const;

    [[nodiscard]] std::uint32_t timerTicks() const;

    [[nodiscard]] std::uint32_t warningCount() const;

    [[nodiscard]] std::uint32_t faultCount() const;

private:
    void handleButtonPressed(const Event &event);

    void handleButtonReleased(const Event &event);

    void handleTimerTick(const Event &event);

    void handleUartCommand(const Event &event);

    void handleSensorError(const Event &event);

    void handleFaultDetected(const Event &event);

    void transitionTo(SystemState nextState);

    void onEnterIdle();

    void onEnterActive();

    void onEnterWarning();

    void onEnterFault();

    void ignoreEvent();

    Led &statusLed_;

    SystemState state_ = SystemState::Idle;

    std::uint32_t handledEvents_ = 0;
    std::uint32_t ignoredEvents_ = 0;
    std::uint32_t timerTicks_ = 0;
    std::uint32_t warningCount_ = 0;
    std::uint32_t faultCount_ = 0;
    std::uint32_t warningBlinkTicks_ = 0;
};