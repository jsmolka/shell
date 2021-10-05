#pragma once

#include <array>

#include <shell/macros.h>
#include <shell/ranges.h>

namespace shell
{

template<typename T, std::size_t kSize>
class RingBufferIterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using reference         = value_type&;
    using pointer           = value_type*;

    RingBufferIterator(T* data, std::size_t rpos, std::size_t size)
        : _data(data), _rpos(rpos), _size(size) {}

    reference operator*()
    {
        return _data[_rpos];
    }

    RingBufferIterator& operator++()
    {
        _size--;
        _rpos = (_rpos + 1) % kSize;
        
        return *this;
    }

    bool operator==(const RingBufferIterator& other) const
    {
        return _data == other._data && _rpos == other._rpos && _size == other._size;
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
    std::size_t _rpos;
    std::size_t _size;
};

template<typename T, std::size_t kSize>
class RingBuffer
{
public:
    using value_type      = T;
    using reference       = value_type&;
    using const_reference = const reference;
    using pointer         = value_type*;
    using const_pointer   = const pointer;
    using iterator        = RingBufferIterator<T, kSize>;
    using const_iterator  = const iterator;
    using sentinel        = Sentinel;
    using const_sentinel  = const sentinel;

    RingBuffer() = default;
    RingBuffer(const RingBuffer<T, kSize>&) = default;
    RingBuffer(RingBuffer<T, kSize>&&) = default;

    RingBuffer(std::initializer_list<T> values)
    {
        for (const auto& value : values)
        {
            write(value);
        }
    }

    reference operator[](std::size_t index)
    {
        SHELL_ASSERT(index < _size);
        return _data[(_rpos + index) % kSize];
    }

    const_reference operator[](std::size_t index) const
    {
        SHELL_ASSERT(index < _size);
        return _data[(_rpos + index) % kSize];
    }

    constexpr std::size_t capacity() const
    {
        return kSize;
    }

    std::size_t size() const
    {
        return _size;
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
        _size = 0;
        _rpos = 0;
        _wpos = 0;
    }

    value_type read()
    {
        SHELL_ASSERT(_size > 0);

        const T& value = _data[_rpos];
        _size--;
        _rpos = (_rpos + 1) % kSize;

        return value;
    }

    void write(const T& value)
    {
        if (_size == kSize)
            _rpos = (_rpos + 1) % kSize;
        else
            _size++;

        _data[_wpos] = value;
        _wpos = (_wpos + 1) % kSize;
    }

    void write(T&& value)
    {
        if (_size == kSize)
            _rpos = (_rpos + 1) % kSize;
        else
            _size++;

        _data[_wpos] = std::move(value);
        _wpos = (_wpos + 1) % kSize;
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
        return (*this)[_size - 1];
    }

    const_reference back() const
    {
        return (*this)[_size - 1];
    }

    SHELL_SENTINEL_ITERATORS(SHELL_ARG(data(), _rpos, _size))

private:
    std::size_t _size = 0;
    std::size_t _rpos = 0;
    std::size_t _wpos = 0;
    std::array<T, kSize> _data = {};
};

}  // namespace shell
