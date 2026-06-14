#pragma once

#include <cstdint>

struct TaskStats {
    std::uint32_t eventsPosted = 0;
    std::uint32_t postFailures = 0;
    std::uint32_t eventsHandled = 0;
    std::uint32_t timeouts = 0;
};