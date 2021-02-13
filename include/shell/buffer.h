#pragma once

#include <memory>
#include <utility>

#include <shell/macros.h>
#include <shell/ranges.h>

namespace shell
{

template<typename T, std::size_t N>
class FixedBuffer
{
public:
    static_assert(N > 0);

    using value_type             = T;
    using reference              = value_type&;
    using const_reference        = const reference;
    using pointer                = value_type*;
    using const_pointer          = const pointer;
    using iterator               = pointer;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    FixedBuffer() = default;

    FixedBuffer(const FixedBuffer<T, N>& other)
    {
        copy(other.begin(), other.end());
    }

    FixedBuffer(FixedBuffer<T, N>&& other)
    {
        copy(other.begin(), other.end());
    }

    FixedBuffer(std::initializer_list<T> values)
    {
        copy(values.begin(), values.end());
    }

    FixedBuffer& operator=(const FixedBuffer<T, N>& other)
    {
        copy(other.begin(), other.end());
        return *this;
    }

    FixedBuffer& operator=(FixedBuffer<T, N>&& other)
    {
        copy(other.begin(), other.end());
        return *this;
    }

    reference operator[](std::size_t index)
    {
        SHELL_ASSERT(index < size_);
        return stack_[index];
    }

    const_reference operator[](std::size_t index) const
    {
        SHELL_ASSERT(index < size_);
        return stack_[index];
    }

    pointer data()
    {
        return stack_;
    }

    const_pointer data() const
    {
        return stack_;
    }

    std::size_t size() const
    {
        return size_;
    }

    constexpr std::size_t capacity() const
    {
        return N;
    }

    void clear()
    {
        size_ = 0;
    }

    void resize(std::size_t size)
    {
        SHELL_ASSERT(size <= N);
        size_ = size;
    }

    void push_back(const T& value)
    {
        SHELL_ASSERT(size_ < N);
        stack_[size_++] = value;
    }

    void push_back(T&& value)
    {
        SHELL_ASSERT(size_ < N);
        stack_[size_++] = std::move(value);
    }

    SHELL_FORWARD_ITERATORS(stack_, stack_ + size_)
    SHELL_REVERSE_ITERATORS(stack_, stack_ + size_)

private:
    template<typename Iterator>
    void copy(Iterator begin, Iterator end)
    {
        std::copy(begin, end, this->begin());

        size_ = std::distance(begin, end);
    }

    T stack_[N];
    std::size_t size_ = 0;
};

template<typename T, std::size_t N>
class SmallBuffer
{
public:
    static_assert(N > 0);

    using value_type             = T;
    using reference              = value_type&;
    using const_reference        = const reference;
    using pointer                = value_type*;
    using const_pointer          = const pointer;
    using iterator               = pointer;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    SmallBuffer() = default;

    SmallBuffer(const SmallBuffer<T, N>& other)
    {
        copy(other.begin(), other.end());
    }

    SmallBuffer(SmallBuffer<T, N>&& other)
    {
        move(std::move(other));
    }

    SmallBuffer(std::initializer_list<T> values)
    {
        copy(values.begin(), values.end());
    }

    ~SmallBuffer()
    {
        if (data_ != stack_)
            delete[] data_;
    }

    SmallBuffer& operator=(const SmallBuffer<T, N>& other)
    {
        copy(other.begin(), other.end());
        return *this;
    }

    SmallBuffer& operator=(SmallBuffer<T, N>&& other)
    {
        move(std::move(other));
        return *this;
    }

    reference operator[](std::size_t index)
    {
        SHELL_ASSERT(index < size_);
        return data_[index];
    }

    const_reference operator[](std::size_t index) const
    {
        SHELL_ASSERT(index < size_);
        return data_[index];
    }

    pointer data()
    {
        return data_;
    }

    const_pointer data() const
    {
        return data_;
    }

    std::size_t size() const
    {
        return size_;
    }

    std::size_t capacity() const
    {
        return capacity_;
    }

    void clear()
    {
        size_ = 0;
    }

    void reserve(std::size_t size)
    {
        if (size > capacity_)
            grow(size);
    }

    void resize(std::size_t size)
    {
        reserve(size);
        size_ = size;
    }

    void push_back(const T& value)
    {
        reserve(size_ + 1);
        data_[size_++] = value;
    }

    void push_back(T&& value)
    {
        reserve(size_ + 1);
        data_[size_++] = std::move(value);
    }

    SHELL_FORWARD_ITERATORS(data_, data_ + size_)
    SHELL_REVERSE_ITERATORS(data_, data_ + size_)

private:
    void grow(std::size_t size)
    {
        std::size_t capacity_old = capacity_;
        std::size_t capacity_new = std::max(2 * capacity_, size);

        T* data_old = data_;
        T* data_new = new T[capacity_new];

        std::uninitialized_copy(begin(), end(), data_new);

        data_ = data_new;
        capacity_ = capacity_new;

        if (data_old != stack_)
            delete[] data_old;
    }

    template<typename Iterator>
    void copy(Iterator begin, Iterator end)
    {
        resize(std::distance(begin, end));

        std::copy(begin, end, this->begin());
    }

    void move(SmallBuffer<T, N>&& other)
    {
        if (other.data_ == other.stack_)
        {
            copy(other.begin(), other.end());
        }
        else
        {
            if (data_ != stack_)
                delete[] data_;

            data_ = other.data_;
            size_ = other.size_;

            other.data_ = other.stack_;
        }
    }

    T  stack_[N];
    T* data_ = stack_;
    std::size_t size_ = 0;
    std::size_t capacity_ = N;
};

}  // namespace shell
