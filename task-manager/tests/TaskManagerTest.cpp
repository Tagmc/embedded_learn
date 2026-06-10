#include <gtest/gtest.h>

#include "app/TaskManager.h"

TEST(TaskManagerTest, AddTaskSuccess) {
    app::TaskManager manager;
    EXPECT_TRUE(manager.addTask(1, "Learning test"));
    const auto task = manager.findById(1);

    ASSERT_TRUE(task.has_value());
    EXPECT_EQ(task->id(), 1);
    EXPECT_EQ(task->title(), "Learning test");
    EXPECT_FALSE(task->isCompleted());
}

TEST(TaskManagerTest, AddTaskFailsWithInvalidId) {
    app::TaskManager manager;

    EXPECT_FALSE(manager.addTask(0, "Invalid id"));
    EXPECT_TRUE(manager.findAll().empty());
}

TEST(TaskManagerTest, AddTaskFailsWithEmptyTitle) {
    app::TaskManager manager;

    EXPECT_FALSE(manager.addTask(1, ""));
    EXPECT_TRUE(manager.findAll().empty());
}

TEST(TaskManagerTest, AddTaskFailsWithDuplicatedId) {
    app::TaskManager manager;

    EXPECT_TRUE(manager.addTask(1, "Task 1"));
    EXPECT_FALSE(manager.addTask(1, "Duplicate task"));

    EXPECT_EQ(manager.findAll().size(), 1);
}

TEST(TaskManagerTest, MarkCompletedSuccess) {
    app::TaskManager manager;

    ASSERT_TRUE(manager.addTask(1, "Task 1"));

    EXPECT_TRUE(manager.markCompleted(1));

    const auto task = manager.findById(1);

    ASSERT_TRUE(task.has_value());
    EXPECT_TRUE(task->isCompleted());
}

TEST(TaskManagerTest, MarkCompletedFailsWhenTaskNotFound) {
    app::TaskManager manager;

    EXPECT_FALSE(manager.markCompleted(999));
}

TEST(TaskManagerTest, ReopenCompletedTask) {
    app::TaskManager manager;

    ASSERT_TRUE(manager.addTask(1, "Task 1"));
    ASSERT_TRUE(manager.markCompleted(1));

    EXPECT_TRUE(manager.reopen(1));

    const auto task = manager.findById(1);

    ASSERT_TRUE(task.has_value());
    EXPECT_FALSE(task->isCompleted());
}