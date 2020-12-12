#pragma once

#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>

namespace shell
{

template<typename T>
class Buffer
{
public:
    using value_type             = T;
    using reference              = value_type&;
    using const_reference        = const reference;
    using pointer                = value_type*;
    using const_pointer          = const pointer;
    using iterator               = pointer;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Buffer() = default;

    std::size_t size() const
    {
        return size_;
    }

    std::size_t capacity() const
    {
        return capacity_;
    }

    T* data()
    {
        return pointer_;
    }
    
    const T* data() const
    {
        return pointer_;
    }

    iterator begin()
    {
        return pointer_;
    }

    iterator end()
    {
        return pointer_ + size_;
    }

    const_iterator begin() const
    {
        return pointer_;
    }

    const_iterator end() const
    {
        return pointer_ + size_;
    }

    const_iterator cbegin() const
    {
        return pointer_;
    }

    const_iterator cend() const
    {
        return pointer_ + size_;
    }

    reverse_iterator rbegin()
    {
        return std::make_reverse_iterator(end());
    }

    reverse_iterator rend()
    {
        return std::make_reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const
    {
        return std::make_reverse_iterator(end());
    }

    const_reverse_iterator rend() const
    {
        return std::make_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const
    {
        return std::make_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const
    {
        return std::make_reverse_iterator(cbegin());
    }

    void clear()
    {
        size_ = 0;
    }

    void resize(std::size_t size)
    {
        reserve(size);
        size_ = size <= capacity_ ? size : capacity_;
    }

    void reserve(std::size_t size)
    {
        if (size > capacity_)
            grow(size);
    }

    void push_back(const T& value)
    {
        reserve(size_ + 1);
        pointer_[size_++] = value;
    }

    void push_back(T&& value)
    {
        reserve(size_ + 1);
        pointer_[size_++] = std::move(value);
    }

    T& operator[](std::size_t index)
    {
        return pointer_[index];
    }

    const T& operator[](std::size_t index) const
    {
        return pointer_[index];
    }

protected:
    Buffer(const Buffer&) = default;
    Buffer(Buffer&&) = default;
    Buffer& operator=(const Buffer&) = default;
    Buffer& operator=(Buffer&&) = default;

    virtual void grow(std::size_t size) = 0;

    void set(T* pointer, std::size_t capacity)
    {
        pointer_ = pointer;
        capacity_ = capacity;
    }

    std::size_t size_ = 0;
    std::size_t capacity_ = 0;

private:
    T* pointer_ = nullptr;
};

template<typename T, std::size_t N>
class FixedBuffer final : public Buffer<T>
{
public:
    static_assert(N > 0);

    using typename Buffer<T>::value_type;
    using typename Buffer<T>::reference;
    using typename Buffer<T>::const_reference;
    using typename Buffer<T>::pointer;
    using typename Buffer<T>::const_pointer;
    using typename Buffer<T>::iterator;
    using typename Buffer<T>::const_iterator;
    using typename Buffer<T>::reverse_iterator;
    using typename Buffer<T>::const_reverse_iterator;

    FixedBuffer()
    {
        this->set(stack_, N);
    }

    FixedBuffer(const FixedBuffer<T, N>& other)
        : FixedBuffer()
    {
        this->copy(other.begin(), other.end());
    }

    FixedBuffer(FixedBuffer<T, N>&& other)
        : FixedBuffer()
    {
        this->copy(other.begin(), other.end());
    }

    FixedBuffer(std::initializer_list<T> value)
        : FixedBuffer()
    {
        this->copy(value.begin(), value.end());
    }

    FixedBuffer& operator=(const FixedBuffer<T, N>& other)
    {
        this->copy(other.begin(), other.end());
        return *this;
    }

    FixedBuffer& operator=(FixedBuffer<T, N>&& other)
    {
        this->copy(other.begin(), other.end());
        return *this;
    }

protected:
    void grow(std::size_t size) final
    {
        throw std::bad_alloc();
    }

private:
    template<typename Iterator>
    void copy(Iterator begin, Iterator end)
    {
        std::copy(begin, end, this->begin());

        this->size_ = std::distance(begin, end);
    }

    T stack_[N];
};

template<typename T, std::size_t N>
class SmallBuffer final : public Buffer<T>
{
public:
    static_assert(N > 0);

    using typename Buffer<T>::value_type;
    using typename Buffer<T>::reference;
    using typename Buffer<T>::const_reference;
    using typename Buffer<T>::pointer;
    using typename Buffer<T>::const_pointer;
    using typename Buffer<T>::iterator;
    using typename Buffer<T>::const_iterator;
    using typename Buffer<T>::reverse_iterator;
    using typename Buffer<T>::const_reverse_iterator;

    SmallBuffer()
    {
        this->set(stack_, N);
    }

    SmallBuffer(const SmallBuffer<T, N>& other)
        : SmallBuffer()
    {
        this->copy(other.begin(), other.end());
    }

    SmallBuffer(SmallBuffer<T, N>&& other)
        : SmallBuffer()
    {
        this->move(std::move(other));
    }

    SmallBuffer(std::initializer_list<T> value)
        : SmallBuffer()
    {
        this->copy(value.begin(), value.end());
    }

    SmallBuffer& operator=(const SmallBuffer<T, N>& other)
    {
        this->copy(other.begin(), other.end());
        return *this;
    }

    SmallBuffer& operator=(SmallBuffer<T, N>&& other)
    {
        this->move(std::move(other));
        return *this;
    }

    ~SmallBuffer()
    {
        if (this->data() != stack_)
            delete[] this->data();
    }

protected:
    void grow(std::size_t size) final
    {
        std::size_t capacity_old = this->capacity();
        std::size_t capacity_new = std::max(capacity_old + capacity_old / 2, size);

        T* data_old = this->data();
        T* data_new = new T[capacity_new];

        std::uninitialized_copy(this->begin(), this->end(), data_new);

        this->set(data_new, capacity_new);

        if (data_old != stack_)
            delete[] data_old;
    }

private:
    template<typename Iterator>
    void copy(Iterator begin, Iterator end)
    {
        std::size_t size = std::distance(begin, end);

        this->reserve(size);
        this->size_ = size;

        std::copy(begin, end, this->begin());
    }

    void move(SmallBuffer<T, N>&& other)
    {
        if (other.data() == other.stack_)
        {
            this->copy(other.begin(), other.end());
        }
        else
        {
            if (this->data() != stack_)
                delete[] this->data();

            this->set(other.data(), other.capacity());
            this->size_ = other.size_;

            other.set(other.stack_, 0);
        }
    }

    T stack_[N];
};

}  // namespace shell
