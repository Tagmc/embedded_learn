#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct LogEntry {
    std::string level;
    std::string service;
    std::string message;
};

std::optional<LogEntry> parseLine(const std::string& line) {
    std::istringstream input(line);

    LogEntry entry;

    if (!(input >> entry.level >> entry.service >> entry.message)) {
        return std::nullopt;
    }

    return entry;
}

std::vector<LogEntry> parseLines(const std::vector<std::string>& lines) {
    std::vector<LogEntry> entries;
    entries.reserve(lines.size());

    for (const std::string& line : lines) {
        std::optional<LogEntry> entry = parseLine(line);

        if (entry.has_value()) {
            entries.push_back(std::move(*entry));
        }
    }

    return entries;
}

std::unordered_map<std::string, int> countByLevel(const std::vector<LogEntry>& entries) {
    std::unordered_map<std::string, int> counts;
    counts.reserve(8);

    for (const LogEntry& entry : entries) {
        counts[entry.level]++;
    }

    return counts;
}

int main() {
    std::vector<std::string> lines;

    lines.reserve(1'000'000);

    for (int i = 0; i < 1'000'000; ++i) {
        lines.push_back("INFO auth login_success");
    }

    const std::vector<LogEntry> entries = parseLines(lines);
    const auto counts = countByLevel(entries);

    for (const auto& [level, count] : counts) {
        std::cout << level << ": " << count << '\n';
    }

    return 0;
}