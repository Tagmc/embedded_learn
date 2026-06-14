#pragma once

#include "GpioTypes.hpp"
#include "Register32.hpp"

#include <cstdint>
#include <optional>

class GpioPort
{
public:
    static constexpr std::uint32_t MaxPins = 16;
    bool configurePin(std::uint32_t pin, GpioMode mode);
    [[nodiscard]] std::optional<GpioMode> modeOf(std::uint32_t pin) const;
    bool writePin(std::uint32_t pin, PinState state);
    bool setPinHigh(std::uint32_t pin);
    bool setPinLow(std::uint32_t pin);
    bool togglePin(std::uint32_t pin);
    [[nodiscard]] bool readPin(std::uint32_t pin) const;
    void simulateInputPin(std::uint32_t pin, PinState state);
    [[nodiscard]] std::uint32_t rawModeRegister() const;
    [[nodiscard]] std::uint32_t rawInputRegister() const;
    [[nodiscard]] std::uint32_t rawOuputRegister() const;

private:
    [[nodiscard]] static bool isValidPin(std::uint32_t pin);
    [[nodiscard]] static std::uint32_t modeShift(std::uint32_t pin);
    [[nodiscard]] static std::uint32_t modeMask(std::uint32_t pin);
    Register32 modeRegister_;
    Register32 inputRegister_;
    Register32 outputRegister_;
};