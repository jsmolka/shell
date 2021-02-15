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
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using reference         = value_type&;
    using pointer           = value_type*;

    RingBufferIterator(T* data, std::size_t index, std::size_t size)
        : data(data), index(index), size(size) {}

    reference operator*()
    {
        return data[index];
    }

    RingBufferIterator& operator++()
    {
        size--;
        index = (index + 1) % N;
        
        return *this;
    }

    bool operator==(const RingBufferIterator& other) const
    {
        return data == other.data && index == other.index && size == other.size;
    }

    bool operator!=(const RingBufferIterator& other) const
    {
        return !(*this == other);
    }

    bool operator==(Sentinel) const
    {
        return size == 0;
    }

    bool operator!=(Sentinel) const
    {
        return !(*this == Sentinel{});
    }

private:
    T* data;
    std::size_t index;
    std::size_t size;
};

template<typename T, std::size_t N>
class RingBuffer
{
public:
    using value_type      = T;
    using reference       = value_type&;
    using const_reference = const reference;
    using pointer         = value_type*;
    using const_pointer   = const pointer;
    using iterator        = RingBufferIterator<T, N>;
    using const_iterator  = const iterator;
    using sentinel        = Sentinel;
    using const_sentinel  = const sentinel;

    RingBuffer() = default;
    RingBuffer(const RingBuffer<T, N>&) = default;
    RingBuffer(RingBuffer<T, N>&&) = default;

    RingBuffer(std::initializer_list<T> values)
    {
        for (const auto& value : values)
        {
            write(value);
        }
    }

    reference operator[](std::size_t index)
    {
        SHELL_ASSERT(index < length);
        return buffer[(rindex + index) % N];
    }

    const_reference operator[](std::size_t index) const
    {
        SHELL_ASSERT(index < length);
        return buffer[(rindex + index) % N];
    }

    constexpr std::size_t capacity() const
    {
        return N;
    }

    std::size_t size() const
    {
        return length;
    }

    pointer data()
    {
        return buffer.data();
    }

    const_pointer data() const
    {
        return buffer.data();
    }

    void clear()
    {
        length = 0;
        rindex = 0;
        windex = 0;
    }

    value_type read()
    {
        SHELL_ASSERT(length > 0);

        const T& value = buffer[rindex];
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

        buffer[windex] = value;
        windex = (windex + 1) % N;
    }

    void write(T&& value)
    {
        if (length == N)
            rindex = (rindex + 1) % N;
        else
            length++;

        buffer[windex] = std::move(value);
        windex = (windex + 1) % N;
    }

    reference front()
    {
        return (*this)[0];
    }

    const_reference front() const
    {
        return (*this)[0];
    }

    reference back()
    {
        return (*this)[length - 1];
    }

    const_reference back() const
    {
        return (*this)[length - 1];
    }

    SHELL_SENTINEL_ITERATORS(SHELL_ARG(data(), rindex, length))

private:
    std::size_t length = 0;
    std::size_t rindex = 0;
    std::size_t windex = 0;
    std::array<T, N> buffer{};
};

}  // namespace shell
