#pragma once

#include <cstdint>

class Register32 {
public:
    explicit Register32(std::uint32_t initialValue = 0);

    [[nodiscard]] std::uint32_t read() const;

    void write(std::uint32_t value);

    void setBits(std::uint32_t mask);

    void clearBits(std::uint32_t mask);

    void toggleBits(std::uint32_t mask);

    [[nodiscard]] bool anyBitsSet(std::uint32_t mask) const;

    [[nodiscard]] bool allBitsSet(std::uint32_t mask) const;

    void writeField(
        std::uint32_t mask,
        std::uint32_t shift,
        std::uint32_t value
    );

    [[nodiscard]] std::uint32_t readField(
        std::uint32_t mask,
        std::uint32_t shift
    ) const;

private:
    std::uint32_t value_ = 0;
};

constexpr std::uint32_t bit(std::uint32_t position) {
    return 1u << position;
}