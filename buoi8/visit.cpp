#include <string>
#include <variant>
#include <iostream>

struct Success
{
    /* data */
    std::string message;
};

struct Error
{
    /* data */
    std::string reason;
};

using Result = std::variant<Success, Error>;

int main()
{
    Result result = Success{"Created Successfully"};
    std::visit(
        [](const auto &value)
        {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, Success>)
            {
                std::cout << "Success: " << value.message << '\n';
            }
            else if constexpr (std::is_same_v<T, Error>)
            {
                std::cout << "Error: " << value.reason << '\n';
            }
        },
        result);
    return 0;
}