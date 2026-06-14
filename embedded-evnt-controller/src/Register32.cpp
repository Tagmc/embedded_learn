#include "Register32.hpp"

Register32::Register32(std::uint32_t initialValue)
    : value_(initialValue) {}

std::uint32_t Register32::read() const {
    return value_;
}

void Register32::write(std::uint32_t value) {
    value_ = value;
}

void Register32::setBits(std::uint32_t mask) {
    value_ |= mask;
}

void Register32::clearBits(std::uint32_t mask) {
    value_ &= ~mask;
}

void Register32::toggleBits(std::uint32_t mask) {
    value_ ^= mask;
}

bool Register32::anyBitsSet(std::uint32_t mask) const {
    return (value_ & mask) != 0;
}

bool Register32::allBitsSet(std::uint32_t mask) const {
    return (value_ & mask) == mask;
}

void Register32::writeField(
    std::uint32_t mask,
    std::uint32_t shift,
    std::uint32_t value
) {
    value_ &= ~mask;
    value_ |= (value << shift) & mask;
}

std::uint32_t Register32::readField(
    std::uint32_t mask,
    std::uint32_t shift
) const {
    return (value_ & mask) >> shift;
}