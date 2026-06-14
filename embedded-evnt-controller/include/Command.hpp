#pragma once

#include <cstdint>
#include <optional>

enum class CommandId : std::uint32_t
{
    LedOn = 1,
    LedOff = 2,
    LedToggle = 3,
    ClearWarning = 4,
    Reset = 5,
    ForceFault = 6
};


inline std::optional<CommandId> commandFromRaw(std::uint32_t raw) {
    switch (raw) {
        case static_cast<std::uint32_t>(CommandId::LedOn):
            return CommandId::LedOn;
        
        case static_cast<std::uint32_t>(CommandId::LedOff):
            return CommandId::LedOff;

        case static_cast<std::uint32_t>(CommandId::LedToggle):
            return CommandId::LedToggle;

        case static_cast<std::uint32_t>(CommandId::ClearWarning):
            return CommandId::ClearWarning;

        case static_cast<std::uint32_t>(CommandId::Reset):
            return CommandId::Reset;

        case static_cast<std::uint32_t>(CommandId::ForceFault):
            return CommandId::ForceFault;

        default:
            return std::nullopt;
    }
}