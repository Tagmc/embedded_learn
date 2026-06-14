#pragma once

#include "GpioPin.hpp"

enum class ButtonActiveLevel {
    ActiveHigh,
    ActiveLow
};

class Button {
public:
    explicit Button(
        GpioPin& pin,
        ButtonActiveLevel activeLevel = ButtonActiveLevel::ActiveHigh
    );

    [[nodiscard]] bool initialize();

    [[nodiscard]] bool isPressed() const;

private:
    GpioPin& pin_;
    ButtonActiveLevel activeLevel_ = ButtonActiveLevel::ActiveHigh;
};