#pragma once

#include "GpioPin.hpp"

class Led {
public:
    explicit Led(GpioPin& pin);

    [[nodiscard]] bool initialize();

    [[nodiscard]] bool turnOn();

    [[nodiscard]] bool turnOff();

    [[nodiscard]] bool toggle();

    [[nodiscard]] bool isOn() const;

private:
    GpioPin& pin_;
};