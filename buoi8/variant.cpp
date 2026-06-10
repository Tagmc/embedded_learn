#include <string>
#include <iostream>
#include <variant>

class User
{
public:
    User(std::string name, int age) : name_(std::move(name)), age_(age) {}
    const std::string &name() const
    {
        return name_;
    }

    int age() const
    {
        return age_;
    }

private:
    std::string name_;
    int age_;
};

enum CreateUserError
{
    EmptyName,
    InvalidAge
};
using CreateUserResult = std::variant<User, CreateUserError>;

CreateUserResult createUser(std::string name, int age)
{
    if (name.empty())
    {
        return CreateUserError::EmptyName;
    }
    if (age <= 0 || age >= 150)
    {
        return CreateUserError::InvalidAge;
    }

    return User(std::move(name), age);
}

std::string errorMessage(CreateUserError error)
{
    switch (error)
    {
    case CreateUserError::EmptyName:
        /* code */
        return "Name must be not empty";
    case CreateUserError::InvalidAge:
        return "Age is invalid";
    }
    return "Unknown error";
}

void printUser(const CreateUserResult& result) {
    if (std::holds_alternative<User>(result)) {
        const User &user = std::get<User>(result);
        std::cout << "Create user: "
                  << user.name() << ', age = ' << user.age() << '\n';
        return;
    }
    const CreateUserError error = std::get<CreateUserError>(result);
    std::cout << "Created user failed: " << errorMessage(error) << '\n';
}

int main()
{
}