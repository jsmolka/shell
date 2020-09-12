#pragma once

#include <eggcpt/traits.h>

namespace eggcpt
{

template<typename Iterator>
class IteratorRange
{
public:
    using iterator = Iterator;
    using const_iterator = const Iterator;

    IteratorRange(Iterator begin, Iterator end)
        : m_begin(begin), m_end(end) {}

    Iterator begin() { return m_begin; }
    Iterator end() { return m_end; }
    const Iterator cbegin() const { return m_begin; }
    const Iterator cend() const { return m_end; }

private:
    Iterator m_begin;
    Iterator m_end;
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

}  // namespace eggcpt
