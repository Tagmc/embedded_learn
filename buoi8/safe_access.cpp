#include <iostream>
#include <vector>
#include <optional>

std::optional<int> getAt(const std::vector<int>& nums, std::size_t index) {
    if (index >= nums.size()) {
        return std::nullopt;
    }
    return nums
}

int main() {
    const std::vector<int> numbers = {1, 2, 3, 4};
}