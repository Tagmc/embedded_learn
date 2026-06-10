#include <string>
#include <variant>
#include <unordered_map>
#include <optional>
#include <utility>
#include <algorithm>
#include <vector>
#include <iostream>

enum class StudentCreateError
{
    InvalidId,
    EmptyName,
    InvalidScore
};

class Student
{
public:
    static std::variant<Student, StudentCreateError> create(int id, std::string name, double score)
    {
        if (id <= 0)
        {
            return StudentCreateError::InvalidId;
        }
        if (name.empty())
        {
            return StudentCreateError::EmptyName;
        }

        if (score < 0 || score > 10)
        {
            return StudentCreateError::InvalidScore;
        }

        return Student(id, std::move(name), score);
    }

    const int &id() const
    {
        return id_;
    }

    const std::string &name() const
    {
        return name_;
    }

    double score() const
    {
        return score_;
    }

    bool updateScore(double newScore)
    {
        if (newScore < 0 || newScore > 10)
        {
            return false;
        }
        score_ = newScore;
        return true;
    }

    bool isPassed() const
    {
        return score_ >= 5.0;
    }

private:
    Student(int id, std::string name, double score) : id_(id), name_(std::move(name)), score_(score) {}
    int id_;
    std::string name_;
    double score_;
};
enum class AddStudentError
{
    InvalidStudent,
    DuplicatedId
};

enum class UpdateScoreError
{
    StudentNotFound,
    InvalidScore
};
class StudentManager
{
public:
    std::optional<AddStudentError> addStudent(int id, std::string name, double score)
    {
        if (students_.find(id) != students_.end())
        {
            return AddStudentError::DuplicatedId;
        }
        const auto result = Student::create(id, std::move(name), score);
        if (std::holds_alternative<StudentCreateError>(result))
        {
            return AddStudentError::InvalidStudent;
        }
        Student student = std::get<Student>(result);
        students_.emplace(student.id(), std::move(student));
        return std::nullopt;
    }
    std::optional<UpdateScoreError> updateScore(int id, double newScore)
    {
        const auto it = students_.find(id);
        if (it == students_.end())
        {
            return UpdateScoreError::StudentNotFound;
        }

        if (!it->second.updateScore(newScore))
        {
            return UpdateScoreError::InvalidScore;
        }
        return std::nullopt;
    }

    std::optional<Student> findById(int id) const
    {
        const auto it = students_.find(id);
        if (it == students_.end())
        {
            return std::nullopt;
        }
        return it->second;
    }

    std::vector<Student> findAll() const
    {
        std::vector<Student> result;
        result.reserve(students_.size());
    }

private:
    std::unordered_map<int, Student> students_;
};

std::string toString(AddStudentError error)
{
    switch (error)
    {
    case AddStudentError::InvalidStudent:
        return "Invalid student data";
    case AddStudentError::DuplicatedId:
        return "Duplicated student id";
    }
    return "Unknown add student error";
}

std::string toString(UpdateScoreError error)
{
    switch (error)
    {
    case UpdateScoreError::StudentNotFound:
        return "Student not found";
    case UpdateScoreError::InvalidScore:
        return "Invalid score";
    }
    return "Unknown update score error";
}

void printStudent(const Student &student)
{
    std::cout << "[" << student.id() << "] "
              << student.name()
              << " - score: "
              << student.score()
              << " - "
              << (student.isPassed() ? "Passed" : "Failed")
              << '\n';
}

int main()
{
    StudentManager manager;

    if (const auto error = manager.addStudent(1, "Huy", 8.5))
    {
        std::cout << toString(*error) << '\n';
    }

    if (const auto error = manager.addStudent(1, "Duplicate", 9.0))
    {
        std::cout << toString(*error) << '\n';
    }

    if (const auto error = manager.addStudent(2, "", 7.0))
    {
        std::cout << toString(*error) << '\n';
    }

    if (const auto error = manager.updateScore(1, 9.5))
    {
        std::cout << toString(*error) << '\n';
    }

    if (const auto error = manager.updateScore(99, 9.5))
    {
        std::cout << toString(*error) << '\n';
    }

    const std::vector<Student> students = manager.findAll();

    for (const Student &student : students)
    {
        printStudent(student);
    }

    return 0;
}