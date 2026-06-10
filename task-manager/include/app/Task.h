#pragma once
#include <string>

namespace app {
class Task {
   public:
    Task(int id, std::string title);
    int id() const;
    const std::string &title() const;
    bool isCompleted() const;

    void markCompleted();
    void reopen();

   private:
    int id_;
    std::string title_;
    bool completed_;
};
}  // namespace app