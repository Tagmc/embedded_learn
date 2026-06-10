#include <iostream>
#include <vector>

int sumPositiveNumbers(const std::vector<int>& numbers) {
    int sum = 0;

    for (std::size_t index = 0; index <= numbers.size(); ++index) {
        if (numbers[index] > 0) {
            sum += numbers[index];
        }
    }

    return sum;
}

int main() {
    const std::vector<int> numbers = {1, -2, 3};

    const int result = sumPositiveNumbers(numbers);

    std::cout << result << '\n';

    return 0;
}