#include <vector>
#include <iostream>
#include <optional>

std::optional<double> calaculateAverage(const std::vector<int> &nums)
{
    if (nums.empty())
    {
        return std::nullopt;
    }
    int sum = 0;
    for (const int num : nums)
    {
        sum += num;
    }
    return static_cast<double>(sum) / nums.size();
}

void printAverage(std::optional<double> average)
{
    if (!average.has_value())
    {
        std::cout << "No data" << '\n';
        return;
    }
    std::cout << "Average: " << average.value() << '\n';
}

int main()
{
    std::vector<int> numbers = {1, 2, 3, 4};
    std::vector<int> emptyNumbers;
}