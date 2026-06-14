#include "GpioPin.hpp"

GpioPin::GpioPin(GpioPort& port, std::uint32_t pin) : port_(port), pin_(pin) {}

bool GpioPin::configureAsInput() {
    return port_.configurePin(pin_, GpioMode::Input);
}
bool GpioPin::configureAsOutput() {
    return port_.configurePin(pin_, GpioMode::Ouput);
}

bool GpioPin::setHigh() {
    return port_.setPinHigh(pin_);
}

bool GpioPin::setLow() {
    return port_.setPinLow(pin_);
}

bool GpioPin::toggle() {
    return port_.togglePin(pin_);
}

bool GpioPin::isHigh() const {
    return port_.readPin(pin_);
}

std::uint32_t GpioPin::number() const {
    return pin_;
}
