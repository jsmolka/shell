#pragma once

#include <memory>
#include <stdexcept>

namespace shell
{

namespace buffer
{

template<typename T>
class Buffer
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;

    Buffer() = default;
    Buffer(Buffer&&) = delete;
    Buffer(const Buffer&) = delete;
    Buffer& operator=(Buffer&&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }

    T* data() { return buffer_; }
    const T* data() const { return buffer_; }

    T* begin() { return buffer_; }
    T* end() { return buffer_ + size_; };
    const T* begin() const { return buffer_; }
    const T* end() const { return buffer_ + size_; }
    const T* cbegin() const { return buffer_; }
    const T* cend() const { return buffer_ + size_; }

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

    void push(const T& value)
    {
        reserve(size_ + 1);
        buffer_[size_++] = value;
    }

    T& operator[](std::size_t index)
    {
        return buffer_[index];
    }

    const T& operator[](std::size_t index) const
    {
        return buffer_[index];
    }

protected:
    void set(T* buffer, std::size_t capacity)
    {
        buffer_ = buffer;
        capacity_ = capacity;
    }

    virtual void grow(std::size_t size) = 0;

private:
    T* buffer_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
};

template<typename T, std::size_t N>
class FixedBuffer final : public Buffer<T>
{
public:
    static_assert(N > 0);

    using typename Buffer<T>::value_type;
    using typename Buffer<T>::reference;
    using typename Buffer<T>::const_reference;
    using typename Buffer<T>::iterator;
    using typename Buffer<T>::const_iterator;

    FixedBuffer()
    {
        this->set(&stack_[0], N);
    }

protected:
    void grow(std::size_t size) final
    {
        throw std::bad_alloc();
    }

private:
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
    using typename Buffer<T>::iterator;
    using typename Buffer<T>::const_iterator;

    SmallBuffer()
    {
        this->set(&stack_[0], N);
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
    T stack_[N];
};

}  // namespace buffer

using buffer::Buffer;
using buffer::FixedBuffer;
using buffer::SmallBuffer;

}  // namespace shell
