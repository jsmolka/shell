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
        : iter_begin(begin), iter_end(end) {}

    Iterator begin() { return iter_begin; }
    Iterator begin() const { return iter_begin; }
    Iterator end() { return iter_end; }
    Iterator end() const { return iter_end; }
    const Iterator cbegin() const { return iter_begin; }
    const Iterator cend() const { return iter_end; }

private:
    Iterator iter_begin;
    Iterator iter_end;
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
