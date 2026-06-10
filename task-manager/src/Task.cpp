#include "app/Task.h"

#include <utility>

namespace app {

Task::Task(int id, std::string title) : id_(id), title_(std::move(title)), completed_(false) {}

int Task::id() const {
    return id_;
}

const std::string& Task::title() const {
    return title_;
}

bool Task::isCompleted() const {
    return completed_;
}

void Task::markCompleted() {
    completed_ = true;
}

void Task::reopen() {
    completed_ = false;
}

}  // namespace app