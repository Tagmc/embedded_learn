#include "GpioPort.hpp"

bool GpioPort::configurePin(std::uint32_t pin, GpioMode mode)
{
    if (!isValidPin(pin))
    {
        return false;
    }

    modeRegister_.writeField(
        modeMask(pin),
        modeShift(pin),
        toValue(mode));
    return true;
}

std::optional<GpioMode> GpioPort::modeOf(std::uint32_t pin) const
{
    if (!isValidPin(pin))
    {
        return std::nullopt;
    }
    const std::uint32_t rawMode = modeRegister_.readField(modeMask(pin), modeShift(pin));

    switch (rawMode)
    {
    case toValue(GpioMode::Input):
        /* code */
        return GpioMode::Input;

    case toValue(GpioMode::Ouput):
        return GpioMode::Ouput;

    default:
        return std::nullopt;
    }
}

bool GpioPort::writePin(std::uint32_t pin, PinState state) {
    if (!isValidPin(pin)) {
        return false;
    }

    const auto mode = modeOf(pin);
    if (!mode.has_value() || mode.value() != GpioMode::Ouput) {
        return false;
    }

    if (state == PinState::High) {
        outputRegister_.setBits(bit(pin));
    } else {
        outputRegister_.clearBits(bit(pin));
    }
    return true;
}

bool GpioPort::setPinHigh(std::uint32_t pin) {
    return writePin(pin, PinState::High);
}

bool GpioPort::setPinLow(std::uint32_t pin) {
    return writePin(pin, PinState::Low);
}

bool GpioPort::togglePin(std::uint32_t pin) {
    if (!isValidPin(pin)) {
        return false;
    }

    const auto mode = modeOf(pin);
    if (!mode.has_value() || mode.value() != GpioMode::Ouput) {
        return false;
    }

    outputRegister_.toggleBits(bit(pin));
    return true;
}

bool GpioPort::readPin(std::uint32_t pin) const {
    if (!isValidPin(pin)) {
        return false;
    }

    const auto mode = modeOf(pin);

    if (mode.has_value() && mode.value() == GpioMode::Input) {
        return inputRegister_.anyBitsSet(bit(pin));
    }

    return outputRegister_.anyBitsSet(bit(pin));
}

void GpioPort::simulateInputPin(std::uint32_t pin, PinState state) {
    if (!isValidPin(pin)) {
        return;
    }

    if (state == PinState::High) {
        inputRegister_.setBits(bit(pin));
    } else {
        inputRegister_.clearBits(bit(pin));
    }
}

std::uint32_t GpioPort::rawModeRegister() const {
    return modeRegister_.read();
}

std::uint32_t GpioPort::rawInputRegister() const {
    return inputRegister_.read();
}

std::uint32_t GpioPort::rawOuputRegister() const {
    return outputRegister_.read();
}

bool GpioPort::isValidPin(std::uint32_t pin) {
    return pin < MaxPins;
}

std::uint32_t GpioPort::modeShift(std::uint32_t pin) {
    return pin * 2u;
}

std::uint32_t GpioPort::modeMask(std::uint32_t pin) {
    return 0b11u << modeShift(pin);
}