#include <iostream>

class IntArray
{
public:
    explicit IntArray(std::size_t size) : size_(size), data_(new int[size])
    {
    }
    ~IntArray()
    {
        delete[] data_;
    }

    IntArray(const IntArray &other) : size_(other.size_), data_(new int[other.size_])
    {
        std::copy(other.data_, other.data_ + other.size_, data_);
    }

    IntArray &operator=(const IntArray &other)
    {
        if (this == &other)
        {
            return *this;
        }
        int *newData = new int[other.size_];
        std::copy(other.data_, other.data_ + other.size_, newData);
        delete[] data_;
        size_ = other.size_;
        data_ = newData;
        return *this;
    }
    int &at(std::size_t index)
    {
        if (index >= size_)
        {
            throw std::out_of_range("index out of range");
        }

        return data_[index];
    }

    const int &at(std::size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("index out of range");
        }

        return data_[index];
    }

    std::size_t size() const
    {
        return size_;
    }

private:
    int *data_;
    size_t size_;
};