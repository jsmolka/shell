#pragma once

#include <memory>
#include <utility>

#include <shell/macros.h>
#include <shell/ranges.h>

namespace shell
{

template<typename T, std::size_t kSize>
class FixedBuffer
{
public:
    static_assert(kSize > 0);

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

    FixedBuffer(const FixedBuffer<T, kSize>& other)
    {
        copy(other.begin(), other.end());
    }

    FixedBuffer(FixedBuffer<T, kSize>&& other)
    {
        copy(other.begin(), other.end());
    }

    FixedBuffer(std::initializer_list<T> values)
    {
        copy(values.begin(), values.end());
    }

    FixedBuffer& operator=(const FixedBuffer<T, kSize>& other)
    {
        copy(other.begin(), other.end());
        return *this;
    }

    FixedBuffer& operator=(FixedBuffer<T, kSize>&& other)
    {
        copy(other.begin(), other.end());
        return *this;
    }

    reference operator[](std::size_t index)
    {
        SHELL_ASSERT(index < _size);
        return _stack[index];
    }

    const_reference operator[](std::size_t index) const
    {
        SHELL_ASSERT(index < _size);
        return _stack[index];
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
        return _stack;
    }

    const_pointer data() const
    {
        return _stack;
    }

    void clear()
    {
        _size = 0;
    }

    void resize(std::size_t size)
    {
        SHELL_ASSERT(size <= kSize);
        _size = size;
    }

    void push_back(const T& value)
    {
        SHELL_ASSERT(_size < kSize);
        _stack[_size++] = value;
    }

    void push_back(T&& value)
    {
        SHELL_ASSERT(_size < kSize);
        _stack[_size++] = std::move(value);
    }

    void pop_back()
    {
        SHELL_ASSERT(_size > 0);
        _size--;
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

    SHELL_FORWARD_ITERATORS(_stack, _stack + _size)
    SHELL_REVERSE_ITERATORS(_stack + _size, _stack)

private:
    template<typename Iterator>
    void copy(Iterator begin, Iterator end)
    {
        std::copy(begin, end, this->begin());

        _size = std::distance(begin, end);
    }

    T _stack[kSize];
    std::size_t _size = 0;
};

template<typename T, std::size_t kSize>
class SmallBuffer
{
public:
    static_assert(kSize > 0);

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

    SmallBuffer(const SmallBuffer<T, kSize>& other)
    {
        copy(other.begin(), other.end());
    }

    SmallBuffer(SmallBuffer<T, kSize>&& other)
    {
        move(std::move(other));
    }

    SmallBuffer(std::initializer_list<T> values)
    {
        copy(values.begin(), values.end());
    }

    ~SmallBuffer()
    {
        if (_data != _stack)
            delete[] _data;
    }

    SmallBuffer& operator=(const SmallBuffer<T, kSize>& other)
    {
        copy(other.begin(), other.end());
        return *this;
    }

    SmallBuffer& operator=(SmallBuffer<T, kSize>&& other)
    {
        move(std::move(other));
        return *this;
    }

    reference operator[](std::size_t index)
    {
        SHELL_ASSERT(index < _size);
        return _data[index];
    }

    const_reference operator[](std::size_t index) const
    {
        SHELL_ASSERT(index < _size);
        return _data[index];
    }

    std::size_t capacity() const
    {
        return _capacity;
    }

    std::size_t size() const
    {
        return _size;
    }

    pointer data()
    {
        return _data;
    }

    const_pointer data() const
    {
        return _data;
    }

    void clear()
    {
        _size = 0;
    }

    void reserve(std::size_t size)
    {
        if (size > _capacity)
            grow(size);
    }

    void resize(std::size_t size)
    {
        reserve(size);
        _size = size;
    }

    void push_back(const T& value)
    {
        reserve(_size + 1);
        _data[_size++] = value;
    }

    void push_back(T&& value)
    {
        reserve(_size + 1);
        _data[_size++] = std::move(value);
    }

    void pop_back()
    {
        SHELL_ASSERT(_size > 0);
        _size--;
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

    SHELL_FORWARD_ITERATORS(_data, _data + _size)
    SHELL_REVERSE_ITERATORS(_data + _size, _data)

private:
    void grow(std::size_t size)
    {
        std::size_t capacity_old = _capacity;
        std::size_t capacity_new = std::max(2 * _capacity, size);

        T* data_old = _data;
        T* data_new = new T[capacity_new];

        std::uninitialized_copy(begin(), end(), data_new);

        _data = data_new;
        _capacity = capacity_new;

        if (data_old != _stack)
            delete[] data_old;
    }

    template<typename Iterator>
    void copy(Iterator begin, Iterator end)
    {
        resize(std::distance(begin, end));

        std::copy(begin, end, this->begin());
    }

    void move(SmallBuffer<T, kSize>&& other)
    {
        if (other._data == other._stack)
        {
            copy(other.begin(), other.end());
        }
        else
        {
            if (_data != _stack)
                delete[] _data;

            _data = other._data;
            _size = other._size;

            other._data = other._stack;
        }
    }

    T _stack[kSize];
    T* _data = _stack;
    std::size_t _size = 0;
    std::size_t _capacity = kSize;
};

}  // namespace shell
