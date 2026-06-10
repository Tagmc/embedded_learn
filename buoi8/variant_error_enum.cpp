#include <string>
#include <iostream>
#include <variant>

enum class UserCreateError
{
    EmptyName,
    InvalidId
};

class User
{
public:
    static std::variant<User, UserCreateError> create(std::string name, int id)
    {
        if (id <= 0)
        {
            return UserCreateError::InvalidId;
        }
        if (name.empty())
        {
            return UserCreateError::EmptyName;
        }

        return User(std::move(name), id);
    }
    const std::string &name() const
    {
        return name_;
    }
    int id() const
    {
        return id_;
    }

private:
    User(std::string name, int id) : name_(name), id_(id) {}
    std::string name_;
    int id_;
};

std::string toString(UserCreateError error)
{
    switch ((error))
    {
    case UserCreateError::EmptyName:
        return "Name can not be empty";
    case UserCreateError::InvalidId:
        return "Id is invalid";
    }
    return "Unknown error";
}

int main()
{
    const auto result = User::create("Huy", 0);
    if (std::holds_alternative<UserCreateError>(result))
    {
        std::cout << toString(std::get<UserCreateError>(result)) << '\n';
        return 1;
    }
    const User &user = std::get<User>(result);
    std::cout << user.name() << " " << user.id() << '\n';
    return 0;
}