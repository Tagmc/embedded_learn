#pragma once

#include "Event.hpp"
#include "RingBuffer.hpp"

#include <cstddef>
#include <optional>

enum class EventQueueStatus
{
    Ok,
    Full
};

template <std::size_t Capaacity>
class EventQueue
{
public:
    [[nodiscard]] EventQueueStatus post(const Event &event)
    {
        if (!events_.push(event))
        {
            return EventQueueStatus::Full;
        }

        return EventQueueStatus::Ok;
    }

    [[nodiscard]] EventQueueStatus post(Event &&event)
    {
        if (!events_.push(std::move(event)))
        {
            return EventQueueStatus::Full;
        }
        return EventQueueStatus::Ok;
    }

    [[nodiscard]] std::optional<Event> tryPop()
    {
        return events_.pop();
    }

    [[nodiscard]] bool empty() const
    {
        return events_.empty();
    }

    [[nodiscard]] bool full() const
    {
        return events_.full();
    }

    [[nodiscard]] std::size_t size() const
    {
        return events_.size();
    }

    [[nodiscard]] constexpr std::size_t capacity() const
    {
        return events_.capacity();
    }

    void clear()
    {
        events_.clear();
    }

private:
    RingBuffer<Event, Capaacity> events_;
};