#include "Led.hpp"

Led::Led(GpioPin& pin)
    : pin_(pin) {}

bool Led::initialize() {
    const bool configured = pin_.configureAsOutput();

    if (!configured) {
        return false;
    }

    return pin_.setLow();
}

bool Led::turnOn() {
    return pin_.setHigh();
}

bool Led::turnOff() {
    return pin_.setLow();
}

bool Led::toggle() {
    return pin_.toggle();
}

bool Led::isOn() const {
    return pin_.isHigh();
}