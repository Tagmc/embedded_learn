#include "app/TaskManager.h"

#include <ostream>
#include <utility>

namespace app {

bool TaskManager::addTask(int id, std::string title) {
    if (id <= 0 || title.empty()) {
        return false;
    }

    if (findTaskById(id) != nullptr) {
        return false;
    }

    tasks_.emplace_back(id, std::move(title));
    return true;
}

bool TaskManager::markCompleted(int id) {
    Task *task = findTaskById(id);

    if (task == nullptr) {
        return false;
    }

    task->markCompleted();
    return true;
}

bool TaskManager::reopen(int id) {
    Task *task = findTaskById(id);

    if (task == nullptr) {
        return false;
    }

    task->reopen();
    return true;
}

std::optional<Task> TaskManager::findById(int id) const {
    const Task *task = findTaskById(id);

    if (task == nullptr) {
        return std::nullopt;
    }

    return *task;
}

std::vector<Task> TaskManager::findAll() const {
    return tasks_;
}

void TaskManager::printAll(std::ostream &output) const {
    for (const Task &task : tasks_) {
        output << "[" << task.id() << "] " << task.title() << " - "
               << (task.isCompleted() ? "Done" : "Pending") << '\n';
    }
}

Task *TaskManager::findTaskById(int id) {
    for (Task &task : tasks_) {
        if (task.id() == id) {
            return &task;
        }
    }

    return nullptr;
}

const Task *TaskManager::findTaskById(int id) const {
    for (const Task &task : tasks_) {
        if (task.id() == id) {
            return &task;
        }
    }

    return nullptr;
}

}  // namespace app