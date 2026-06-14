#pragma once

#include "Command.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

enum class CommandParseStatus
{
    InProgress,
    CommandReady,
    InvalidCommand,
    BufferOverflow,
    EmptyLine
};

struct CommandParseResult {
    CommandParseStatus status = CommandParseStatus::InProgress;
    std::optional<CommandId> command = std::nullopt;
};

class CommandParser {
    public:
        static constexpr std::size_t MaxCommandLength = 32;
        [[nodiscard]] CommandParseResult feed(std::uint8_t byte);
        void reset();
        [[nodiscard]] std::size_t size() const;
        [[nodiscard]] constexpr std::size_t capacity() const {
            return MaxCommandLength;
        }
    private:
        [[nodiscard]] CommandParseResult finalizeLine();
        [[nodiscard]] std::optional<CommandId> matchCommand() const;
        [[nodiscard]] bool equals(const char *text) const;
        void append(char ch);
        std::array<char, MaxCommandLength> buffer_{};
        std::size_t size_ = 0;
};