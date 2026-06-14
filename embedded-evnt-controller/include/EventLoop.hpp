#pragma once

#include "EventQueue.hpp"

#include <cstddef>

template <std::size_t QueueCapacity, typename Handler>
class EventLoop {
public:
    EventLoop(EventQueue<QueueCapacity>& eventQueue, Handler& handler)
        : eventQueue_(eventQueue),
          handler_(handler) {}

    std::size_t processPendingEvents(std::size_t maxEventsToProcess) {
        std::size_t processed = 0;

        while (processed < maxEventsToProcess) {
            auto event = eventQueue_.tryPop();

            if (!event.has_value()) {
                break;
            }

            handler_.handle(*event);
            ++processed;
        }

        return processed;
    }

    std::size_t processAllPendingEvents() {
        std::size_t processed = 0;

        while (true) {
            auto event = eventQueue_.tryPop();

            if (!event.has_value()) {
                break;
            }

            handler_.handle(*event);
            ++processed;
        }

        return processed;
    }

private:
    EventQueue<QueueCapacity>& eventQueue_;
    Handler& handler_;
};