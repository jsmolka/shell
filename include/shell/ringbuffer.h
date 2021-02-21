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
        : _data(data), _index(index), _size(size) {}

    reference operator*()
    {
        return _data[_index];
    }

    RingBufferIterator& operator++()
    {
        _size--;
        _index = (_index + 1) % N;
        
        return *this;
    }

    bool operator==(const RingBufferIterator& other) const
    {
        return _data == other._data && _index == other._index && _size == other._size;
    }

    bool operator!=(const RingBufferIterator& other) const
    {
        return !(*this == other);
    }

    bool operator==(Sentinel) const
    {
        return _size == 0;
    }

    bool operator!=(Sentinel) const
    {
        return !(*this == Sentinel{});
    }

private:
    T* _data;
    std::size_t _index;
    std::size_t _size;
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
        SHELL_ASSERT(index < _length);
        return _data[(_rindex + index) % N];
    }

    const_reference operator[](std::size_t index) const
    {
        SHELL_ASSERT(index < _length);
        return _data[(_rindex + index) % N];
    }

    constexpr std::size_t capacity() const
    {
        return N;
    }

    std::size_t size() const
    {
        return _length;
    }

    pointer data()
    {
        return _data.data();
    }

    const_pointer data() const
    {
        return _data.data();
    }

    void clear()
    {
        _length = 0;
        _rindex = 0;
        _windex = 0;
    }

    value_type read()
    {
        SHELL_ASSERT(_length > 0);

        const T& value = _data[_rindex];
        _length--;
        _rindex = (_rindex + 1) % N;

        return value;
    }

    void write(const T& value)
    {
        if (_length == N)
            _rindex = (_rindex + 1) % N;
        else
            _length++;

        _data[_windex] = value;
        _windex = (_windex + 1) % N;
    }

    void write(T&& value)
    {
        if (_length == N)
            _rindex = (_rindex + 1) % N;
        else
            _length++;

        _data[_windex] = std::move(value);
        _windex = (_windex + 1) % N;
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
        return (*this)[_length - 1];
    }

    const_reference back() const
    {
        return (*this)[_length - 1];
    }

    SHELL_SENTINEL_ITERATORS(SHELL_ARG(data(), _rindex, _length))

private:
    std::size_t _length = 0;
    std::size_t _rindex = 0;
    std::size_t _windex = 0;
    std::array<T, N> _data{};
};

}  // namespace shell
