#include "Button.hpp"

Button::Button(GpioPin& pin, ButtonActiveLevel activeLevel)
    : pin_(pin),
      activeLevel_(activeLevel) {}

bool Button::initialize() {
    return pin_.configureAsInput();
}

bool Button::isPressed() const {
    const bool high = pin_.isHigh();

    if (activeLevel_ == ButtonActiveLevel::ActiveHigh) {
        return high;
    }

    return !high;
}