#include "CommandParser.hpp"

namespace {
char toUpperAscii(char ch) {
    if (ch >= 'a' && ch <= 'z') {
        return static_cast<char>(ch - 'a' + 'A');
    }

    return ch;
}
}

CommandParseResult CommandParser::feed(std::uint8_t byte) {
    const char ch = static_cast<char>(byte);

    if (ch == '\r') {
        return CommandParseResult{
            CommandParseStatus::InProgress,
            std::nullopt
        };
    }

    if (ch == '\n') {
        return finalizeLine();
    }

    if (size_ >= MaxCommandLength) {
        reset();

        return CommandParseResult{
            CommandParseStatus::BufferOverflow,
            std::nullopt
        };
    }

    append(toUpperAscii(ch));

    return CommandParseResult{
        CommandParseStatus::InProgress,
        std::nullopt
    };
}

void CommandParser::reset() {
    size_ = 0;
}

std::size_t CommandParser::size() const {
    return size_;
}

CommandParseResult CommandParser::finalizeLine() {
    if (size_ == 0) {
        return CommandParseResult{
            CommandParseStatus::EmptyLine,
            std::nullopt
        };
    }

    const auto command = matchCommand();

    reset();

    if (!command.has_value()) {
        return CommandParseResult{
            CommandParseStatus::InvalidCommand,
            std::nullopt
        };
    }

    return CommandParseResult{
        CommandParseStatus::CommandReady,
        command
    };
}

std::optional<CommandId> CommandParser::matchCommand() const {
    if (equals("LED ON")) {
        return CommandId::LedOn;
    }

    if (equals("LED OFF")) {
        return CommandId::LedOff;
    }

    if (equals("LED TOGGLE")) {
        return CommandId::LedToggle;
    }

    if (equals("CLEAR WARNING")) {
        return CommandId::ClearWarning;
    }

    if (equals("RESET")) {
        return CommandId::Reset;
    }

    if (equals("FAULT")) {
        return CommandId::ForceFault;
    }

    if (equals("FORCE FAULT")) {
        return CommandId::ForceFault;
    }

    return std::nullopt;
}

bool CommandParser::equals(const char* text) const {
    std::size_t index = 0;

    while (text[index] != '\0') {
        if (index >= size_) {
            return false;
        }

        if (buffer_[index] != text[index]) {
            return false;
        }

        ++index;
    }

    return index == size_;
}

void CommandParser::append(char ch) {
    buffer_[size_] = ch;
    ++size_;
}