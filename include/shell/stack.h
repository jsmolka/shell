#pragma once

#include <shell/vector.h>

namespace shell
{

template<typename T, std::size_t kSize = vector_sso_v<T>>
class Stack : private Vector<T, kSize>
{
public:
    using Base = Vector<T, kSize>;
    using typename Base::value_type;
    using typename Base::reference;
    using typename Base::const_reference;
    using typename Base::pointer;
    using typename Base::const_pointer;
    using typename Base::iterator;
    using typename Base::const_iterator;
    using typename Base::reverse_iterator;
    using typename Base::const_reverse_iterator;
    using Base::Base;
    using Base::operator=;
    using Base::operator[];
    using Base::capacity;
    using Base::size;
    using Base::empty;
    using Base::data;
    using Base::clear;
    using Base::begin;
    using Base::end;
    using Base::cbegin;
    using Base::cend;
    using Base::rbegin;
    using Base::rend;
    using Base::crbegin;
    using Base::crend;

    void push(const T& value)
    {
        this->push_back(value);
    }

    void push(T&& value)
    {
        this->push_back(std::move(value));
    }

    void pop(std::size_t count = 1)
    {
        this->pop_back(count);
    }

    value_type popValue(std::size_t count = 1)
    {
        this->pop_back(count);
        return std::move(*this->_head);
    }

    reference peek(std::size_t index)
    {
        return *(this->_head - index - 1);
    }

    const_reference peek(std::size_t index) const
    {
        return *(this->_head - index - 1);
    }

    reference top()
    {
        return this->back();
    }

    const_reference top() const
    {
        return this->back();
    }
};

}  // namespace shell
