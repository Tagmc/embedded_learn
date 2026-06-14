#pragma once

#include "GpioPort.hpp"
#include "GpioTypes.hpp"

#include <cstdint>

class GpioPin {
    public:
        GpioPin(GpioPort &port, std::uint32_t pin);
        [[nodiscard]] bool configureAsInput();
        [[nodiscard]] bool configureAsOutput();
        [[nodiscard]] bool setHigh();
        [[nodiscard]] bool setLow();
        [[nodiscard]] bool toggle();
        [[nodiscard]] bool isHigh() const;
        [[nodiscard]] std::uint32_t number() const;
    private:
        GpioPort &port_;
        std::uint32_t pin_ = 0;
};