#pragma once

#include <array>

#include <shell/macros.h>
#include <shell/ranges.h>

namespace shell
{

template<typename T, std::size_t N>
class RingBufferIterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using reference         = T&;
    using pointer           = T*;

    RingBufferIterator(T* begin, std::size_t index)
        : begin(begin), index(index) {}

    reference operator*()
    {
        return begin[index];
    }

    RingBufferIterator& operator++()
    {
        index = (index + 1) % N;
        return *this;
    }

    RingBufferIterator& operator--()
    {
        index = (index - 1) % N;
        return *this;
    }

    bool operator==(const RingBufferIterator& other) const
    {
        return begin == other.begin && index == other.index;
    }

    bool operator!=(const RingBufferIterator& other) const
    {
        return begin != other.begin || index != other.index;
    }

private:
    T* begin;
    std::size_t index;
};

template<typename T, std::size_t N>
class RingBuffer
{
public:
    using value_type             = T;
    using reference              = value_type&;
    using const_reference        = const reference;
    using pointer                = value_type*;
    using const_pointer          = const pointer;
    using iterator               = RingBufferIterator<T, N>;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr std::size_t capacity() const
    {
        return N;
    }

    std::size_t size() const
    {
        return length;
    }

    reference read()
    {
        SHELL_ASSERT(length > 0);

        T& value = data[rindex];
        length--;
        rindex = (rindex + 1) % N;
        return value;
    }

    void write(const T& value)
    {
        if (length == N)
            rindex = (rindex + 1) % N;
        else
            length++;

        data[windex] = value;
        windex = (windex + 1) % N;
    }

    void write(T&& value)
    {
        if (length == N)
            rindex = (rindex + 1) % N;
        else
            length++;

        data[windex] = std::move(value);
        windex = (windex + 1) % N;
    }

    reference front()
    {
        SHELL_ASSERT(length > 0);

        return data[rindex];
    }

    const_reference front() const
    {
        SHELL_ASSERT(length > 0);

        return data[rindex];
    }

    reference back()
    {
        SHELL_ASSERT(length > 0);

        return data[(rindex + length - 1) % N];
    }

    const_reference back() const
    {
        SHELL_ASSERT(length > 0);

        return data[(rindex + length - 1) % N];
    }

    void clear()
    {
        length = 0;
        rindex = 0;
        windex = 0;
    }

    SHELL_FORWARD_ITERATORS(
        SHELL_ARG(data.data(), rindex),
        SHELL_ARG(data.data(), (windex + length) % N));
    SHELL_REVERSE_ITERATORS(end(), begin());

private:
    std::size_t length = 0;
    std::size_t rindex = 0;
    std::size_t windex = 0;
    std::array<T, N> data{};
};

}  // namespace shell
