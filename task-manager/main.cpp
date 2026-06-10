#include <iostream>

#include "app/TaskManager.h"

int main() {
    app::TaskManager manager;

    if (!manager.addTask(1, "Learn CMake")) {
        std::cout << "Failed to add task 1\n";
    }

    if (!manager.addTask(2, "Split header and source")) {
        std::cout << "Failed to add task 2\n";
    }

    if (!manager.addTask(1, "Duplicate task")) {
        std::cout << "Failed to add duplicate task\n";
    }

    manager.markCompleted(1);

    manager.printAll(std::cout);

    return 0;
}