#pragma once

#include <iterator>

#include <shell/traits.h>

namespace shell
{

template<typename Iterator>
class IteratorRange
{
public:
    using iterator = Iterator;
    using const_iterator = const Iterator;

    IteratorRange(Iterator begin, Iterator end)
        : begin_(begin), end_(end) {}

    iterator begin()
    {
        return begin_;
    }

    iterator end()
    {
        return end_;
    }

    const_iterator begin() const
    {
        return begin_;
    }
    
    const_iterator end() const
    {
        return end_;
    }
    
    const_iterator cbegin() const
    {
        return begin_;
    }
    
    const_iterator cend() const
    {
        return end_;
    }

private:
    Iterator begin_;
    Iterator end_;
};

template<typename T>
class PointerRange
{
public:
    using iterator = T*;
    using const_iterator = const iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    PointerRange(T* begin, T* end)
        : begin_(begin), end_(end) {}

    PointerRange(T* begin, std::size_t size)
        : begin_(begin), end_(begin + size) {}

    iterator begin()
    {
        return begin_;
    }

    iterator end()
    {
        return end_;
    }

    const_iterator begin() const
    {
        return begin_;
    }

    const_iterator end() const
    {
        return end_;
    }

    const_iterator cbegin() const
    {
        return begin_;
    }

    const_iterator cend() const
    {
        return end_;
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
        return std::make_reverse_iterator(end());
    }

    const_reverse_iterator crend() const
    {
        return std::make_reverse_iterator(begin());
    }

private:
    T* begin_;
    T* end_;
};

template<typename Range>
IteratorRange<range_reverse_iterator_t<Range>> reversed(Range& range)
{
    return IteratorRange(std::rbegin(range), std::rend(range));
}

}  // namespace shell
