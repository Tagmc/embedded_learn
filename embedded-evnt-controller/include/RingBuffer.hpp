#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <utility>

template <typename T, std::size_t Capacity>
class RingBuffer
{
public:
    static_assert(Capacity > 0, "RingBuffer capacity must be greater than zero");
    [[nodiscard]] bool push(const T &value)
    {
        if (full())
        {
            return false;
        }

        data_[head_] = value;
        head_ = nextIndex(head_);
        ++size_;
        return true;
    }
    [[nodiscard]] bool push(T &&value)
    {
        if (full())
        {
            return false;
        }

        data_[head_] = std::move(value);
        head_ = nextIndex(head_);
        ++size_;
        return true;
    }

    [[nodiscard]] std::optional<T> pop()
    {
        if (empty())
        {
            return std::nullopt;
        }

        T value = std::move(data_[tail_]);

        tail_ = nextIndex(tail_);
        --size_;

        return value;
    }

    [[nodiscard]] std::optional<T> front() const
    {
        if (empty())
        {
            return std::nullopt;
        }

        return data_[tail_];
    }

    void clear()
    {
        head_ = 0;
        tail_ = 0;
        size_ = 0;
    }

    [[nodiscard]] bool empty() const
    {
        return size_ == 0;
    }

    [[nodiscard]] bool full() const
    {
        return size_ == Capacity;
    }

    [[nodiscard]] std::size_t size() const
    {
        return size_;
    }

    [[nodiscard]] constexpr std::size_t capacity() const
    {
        return Capacity;
    }

private:
    [[nodiscard]] constexpr std::size_t nextIndex(std::size_t index) const
    {
        return (index + 1) % Capacity;
    }

    std::array<T, Capacity> data_{};
    std::size_t head_ = 0;
    std::size_t tail_ = 0;
    std::size_t size_ = 0;
};