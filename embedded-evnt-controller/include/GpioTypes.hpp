#pragma once

#include <cstdint>

enum class GpioMode : std::uint32_t
{
    Input = 0b00,
    Ouput = 0b01
};

enum class PinState : std::uint32_t
{
    Low = 0,
    High = 1
};

constexpr std::uint32_t toValue(GpioMode mode)
{
    return static_cast<std::uint32_t>(mode);
}

constexpr std::uint32_t toValue(PinState state)
{
    return static_cast<std::uint32_t>(state);
}