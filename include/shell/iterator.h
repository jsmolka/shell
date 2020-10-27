#pragma once

#include <shell/traits.h>

namespace shell
{

namespace iterator
{

template<typename Iterator>
class IteratorRange
{
public:
    using iterator = Iterator;
    using const_iterator = const Iterator;

    IteratorRange(Iterator begin, Iterator end)
        : begin_(begin), end_(end) {}

    Iterator begin() { return begin_; }
    Iterator end() { return end_; }
    const Iterator begin() const { return begin_; }
    const Iterator end() const { return end_; }
    const Iterator cbegin() const { return begin_; }
    const Iterator cend() const { return end_; }

private:
    Iterator begin_;
    Iterator end_;
};

template<typename Iterator>
IteratorRange<Iterator> 
    makeIteratorRange(Iterator begin, Iterator end)
{
    return IteratorRange<Iterator>(begin, end);
}

template<typename Range>
IteratorRange<range_reverse_iterator_t<Range>>
    reversed(Range& range)
{
    return makeIteratorRange(
        std::rbegin(range),
        std::rend(range));
}

}  // namespace iterator

using iterator::IteratorRange;
using iterator::makeIteratorRange;
using iterator::reversed;

}  // namespace shell
