#include <string>
#include <iostream>
#include <optional>

class User
{
public:
    User(std::string name, int age) : name_(std::move(name)), age_(age) {};
    const std::string &name() const
    {
        return name_;
    }
    int age()
    {
        return age_;
    }

private:
    std::string name_;
    int age_;
};

enum class CreateUserError
{
    EmptyName,
    InvalidAge
};

struct CreateUserResult
{
    std::optional<User> user;
    std::optional<CreateUserError> error;
};

CreateUserResult createUser(std::string name, int age)
{
    if (name.empty())
    {
        return CreateUserResult{
            std::nullopt,
            CreateUserError::EmptyName};
    }
    if (age <= 0 || age >= 150)
    {
        return CreateUserResult{
            std::nullopt,
            CreateUserError::InvalidAge};
    }

    return CreateUserResult{
        User(std::move(name), age),
        std::nullopt};
}

std::string toString(CreateUserError error) {
    switch (error)
    {
    case CreateUserError::EmptyName:
        /* code */
        return "Name cannot be empty";
    case CreateUserError::InvalidAge:
        return "Age is invalid";
    }
    return "Unkown error";
}

int main() {
    const CreateUserResult result = createUser("", 20);
    if (result.error.has_value()) {
        std::cout << toString(result.error.value()) << '\n';
        return 1;
    }
    std::cout << "User: " << result.user->name() << '\n';
    return 0;
}