#include "SimpleController.hpp"

#include <iostream>

SimpleController::SimpleController(Led& statusLed)
    : statusLed_(statusLed) {}

void SimpleController::handle(const Event& event) {
    ++handledEvents_;

    switch (event.type) {
        case EventType::ButtonPressed:
            handleButtonPressed(event);
            break;

        case EventType::ButtonReleased:
            handleButtonReleased(event);
            break;

        case EventType::TimerTick:
            handleTimerTick(event);
            break;

        case EventType::UartCommandRecieved:
            handleUartCommand(event);
            break;

        case EventType::SensorError:
            handleSensorError(event);
            break;

        case EventType::FaultDetected:
            handleFaultDetected(event);
            break;
    }
}

std::uint32_t SimpleController::handledEvents() const {
    return handledEvents_;
}

std::uint32_t SimpleController::timerTicks() const {
    return timerTicks_;
}

std::uint32_t SimpleController::sensorErrors() const {
    return sensorErrors_;
}

void SimpleController::handleButtonPressed(const Event& event) {
    std::cout << "[controller] ButtonPressed at "
              << event.timestampMs
              << " ms -> toggle LED\n";

    statusLed_.toggle();
}

void SimpleController::handleButtonReleased(const Event& event) {
    std::cout << "[controller] ButtonReleased at "
              << event.timestampMs
              << " ms\n";
}

void SimpleController::handleTimerTick(const Event& event) {
    ++timerTicks_;

    std::cout << "[controller] TimerTick #"
              << timerTicks_
              << " at "
              << event.timestampMs
              << " ms\n";

    if (timerTicks_ % 5 == 0) {
        std::cout << "[controller] 5 ticks reached -> toggle LED\n";
        statusLed_.toggle();
    }
}

void SimpleController::handleUartCommand(const Event& event) {
    std::cout << "[controller] UartCommandReceived data="
              << event.data
              << " at "
              << event.timestampMs
              << " ms\n";

    switch (event.data) {
        case 1:
            std::cout << "[controller] UART command: LED ON\n";
            statusLed_.turnOn();
            break;

        case 2:
            std::cout << "[controller] UART command: LED OFF\n";
            statusLed_.turnOff();
            break;

        case 3:
            std::cout << "[controller] UART command: LED TOGGLE\n";
            statusLed_.toggle();
            break;

        default:
            std::cout << "[controller] Unknown UART command\n";
            break;
    }
}

void SimpleController::handleSensorError(const Event& event) {
    ++sensorErrors_;

    std::cout << "[controller] SensorError code="
              << event.data
              << " at "
              << event.timestampMs
              << " ms -> LED ON\n";

    statusLed_.turnOn();
}

void SimpleController::handleFaultDetected(const Event& event) {
    std::cout << "[controller] FaultDetected code="
              << event.data
              << " at "
              << event.timestampMs
              << " ms -> LED ON\n";

    statusLed_.turnOn();
}