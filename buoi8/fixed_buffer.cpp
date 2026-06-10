#include <array>
#include <optional>
#include <variant>
#include <iostream>

enum class BufferError
{
    Full,
    InvalidIndex
};
class FixedBuffer
{
    public:
        std::optional<BufferError> push(int value) {
            if (size_ >= data_.size()) {
                return BufferError::Full;
            }
            data_[size_] = value;
            ++size_;
            return std::nullopt;
        }

        std::variant<int, BufferError> get(std::size_t index) const {
            if (index >= size_) {
                return BufferError::InvalidIndex;
            }
            return data_[index];
        }

        std::size_t size() const {
            return size_;
        }

    private:
        std::array<int, 4> data_{};
        std ::size_t size_ = 0;
};

std::string toString(BufferError error) {
    switch (error) {
        case BufferError::Full:
            return "Buffer is full";
        case BufferError::InvalidIndex:
            return "Invalid index";
    }

    return "Unknown buffer error";
}

int main() {
    FixedBuffer buffer;

    for (int value = 1; value <= 5; ++value) {
        const auto error = buffer.push(value * 10);

        if (error.has_value()) {
            std::cout << "Push failed: " << toString(*error) << '\n';
        }
    }

    const auto result = buffer.get(10);

    if (std::holds_alternative<BufferError>(result)) {
        std::cout << "Get failed: "
                  << toString(std::get<BufferError>(result))
                  << '\n';
    }

    return 0;
}