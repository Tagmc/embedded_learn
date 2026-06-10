#pragma once

#include <iosfwd>
#include <optional>
#include <vector>

#include "app/Task.h"

namespace app {

class TaskManager {
   public:
    bool addTask(int id, std::string title);
    bool markCompleted(int id);
    bool reopen(int id);

    std::optional<Task> findById(int id) const;
    std::vector<Task> findAll() const;

    void printAll(std::ostream& output) const;

   private:
    Task* findTaskById(int id);
    const Task* findTaskById(int id) const;

    std::vector<Task> tasks_;
};

}  // namespace app