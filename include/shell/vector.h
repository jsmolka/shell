#pragma once

#include <memory>
#include <utility>

#include <shell/constants.h>
#include <shell/macros.h>
#include <shell/ranges.h>

namespace shell
{

template<typename T, std::size_t kSize>
class FixedVector
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

    FixedVector() = default;

    FixedVector(const FixedVector<T, kSize>& other)
    {
        copy(other.begin(), other.end());
    }

    FixedVector(FixedVector<T, kSize>&& other)
    {
        copy(
            std::make_move_iterator(other.begin()),
            std::make_move_iterator(other.end()));
    }

    FixedVector(std::initializer_list<T> values)
    {
        copy(values.begin(), values.end());
    }

    FixedVector& operator=(const FixedVector<T, kSize>& other)
    {
        copy(other.begin(), other.end());
        return *this;
    }

    FixedVector& operator=(FixedVector<T, kSize>&& other)
    {
        copy(
            std::make_move_iterator(other.begin()),
            std::make_move_iterator(other.end()));
        return *this;
    }

    constexpr std::size_t capacity() const
    {
        return kSize;
    }

    std::size_t size() const
    {
        return _head - _stack;
    }

    bool empty() const
    {
        return _head == _stack;
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
        _head = _stack;
    }

    void resize(std::size_t size)
    {
        SHELL_ASSERT(size <= kSize);
        _head = _stack + size;
    }

    void push_back(const T& value)
    {
        SHELL_ASSERT(size() < kSize);
        *_head++ = value;
    }

    void push_back(T&& value)
    {
        SHELL_ASSERT(size() < kSize);
        *_head++ = std::move(value);
    }

    void pop_back(std::size_t count = 1)
    {
        SHELL_ASSERT(_stack + count <= _head);
        _head -= count;
    }

    reference front()
    {
        SHELL_ASSERT(size() > 0);
        return *_stack;
    }

    const_reference front() const
    {
        SHELL_ASSERT(size() > 0);
        return *_stack;
    }

    reference back()
    {
        SHELL_ASSERT(size() > 0);
        return _head[-1];
    }

    const_reference back() const
    {
        SHELL_ASSERT(size() > 0);
        return _head[-1];
    }

    reference operator[](std::size_t index)
    {
        SHELL_ASSERT(index < size());
        return _stack[index];
    }

    const_reference operator[](std::size_t index) const
    {
        SHELL_ASSERT(index < size());
        return _stack[index];
    }

    SHELL_FORWARD_ITERATORS(_stack, _head)
    SHELL_REVERSE_ITERATORS(_head, _stack)

private:
    template<typename Iterator>
    void copy(Iterator begin, Iterator end)
    {
        _head = std::copy(begin, end, this->begin());
    }

    T* _head = _stack;
    T _stack[kSize];
};

template<typename T>
struct vector_sso : std::integral_constant<std::size_t,
    std::max<std::size_t>(1, (8 * kCacheLine - 3 * sizeof(T*)) / sizeof(T))> {};

template<typename T>
inline constexpr std::size_t vector_sso_v = vector_sso<T>::value;

template<typename T, std::size_t kSize = vector_sso_v<T>>
class Vector
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

    Vector() = default;

    Vector(const Vector<T, kSize>& other)
    {
        copy(other.begin(), other.end());
    }

    Vector(Vector<T, kSize>&& other)
    {
        move(std::move(other));
    }

    Vector(std::initializer_list<T> values)
    {
        copy(values.begin(), values.end());
    }

    ~Vector()
    {
        if (_data != _stack)
            delete[] _data;
    }

    Vector& operator=(const Vector<T, kSize>& other)
    {
        copy(other.begin(), other.end());
        return *this;
    }

    Vector& operator=(Vector<T, kSize>&& other)
    {
        move(std::move(other));
        return *this;
    }

    std::size_t capacity() const
    {
        return _last - _data;
    }

    std::size_t size() const
    {
        return _head - _data;
    }

    bool empty() const
    {
        return _head == _stack;
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
        _head = _data;
    }

    void reserve(std::size_t size)
    {
        if (size > capacity())
            grow(size);
    }

    void resize(std::size_t size)
    {
        reserve(size);
        _head = _data + size;
    }

    void push_back(const T& value)
    {
        if (_head == _last)
            grow(0);
        *_head++ = value;
    }

    void push_back(T&& value)
    {
        if (_head == _last)
            grow(0);
        *_head++ = std::move(value);
    }

    void pop_back(std::size_t count = 1)
    {
        SHELL_ASSERT(_data + count <= _head);
        _head -= count;
    }

    reference front()
    {
        SHELL_ASSERT(size() > 0);
        return *_data;
    }

    const_reference front() const
    {
        SHELL_ASSERT(size() > 0);
        return *_data;
    }

    reference back()
    {
        SHELL_ASSERT(size() > 0);
        return _head[-1];
    }

    const_reference back() const
    {
        SHELL_ASSERT(size() > 0);
        return _head[-1];
    }

    reference operator[](std::size_t index)
    {
        SHELL_ASSERT(index < size());
        return _data[index];
    }

    const_reference operator[](std::size_t index) const
    {
        SHELL_ASSERT(index < size());
        return _data[index];
    }

    SHELL_FORWARD_ITERATORS(_data, _head)
    SHELL_REVERSE_ITERATORS(_head, _data)

protected:
    SHELL_NO_INLINE void grow(std::size_t size)
    {
        std::size_t capacity_old = capacity();
        std::size_t capacity_new = std::max(2 * capacity_old, size);

        T* data_old = _data;
        T* data_new = new T[capacity_new];

        _head = std::copy(
            std::make_move_iterator(begin()),
            std::make_move_iterator(end()),
            data_new);
        _data = data_new;
        _last = data_new + capacity_new;

        if (data_old != _stack)
            delete[] data_old;
    }

    template<typename Iterator>
    void copy(Iterator begin, Iterator end)
    {
        reserve(std::distance(begin, end));

        _head = std::copy(begin, end, this->begin());
    }

    void move(Vector<T, kSize>&& other)
    {
        if (other._data == other._stack)
        {
            copy(
                std::make_move_iterator(other.begin()),
                std::make_move_iterator(other.end()));
        }
        else
        {
            if (_data != _stack)
                delete[] _data;

            _data = other._data;
            _head = other._head;
            _last = other._last;

            other._data = other._stack;
        }
    }

    T* _data = _stack;
    T* _head = _stack;
    T* _last = _stack + kSize;
    T _stack[kSize];
};

}  // namespace shell
