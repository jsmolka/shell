#pragma once

#include <iterator>

namespace eggcpt
{

template<typename Iterator>
class IteratorRange
{
public:
    using iterator = Iterator;
    using const_iterator = const Iterator;

    IteratorRange(iterator begin, iterator end)
        : begin_(begin), end_(end) {}

    iterator begin() { return begin_; }
    iterator end() { return end_; }
    const_iterator cbegin() const { return begin_; }
    const_iterator cend() const { return end_; }

private:
    iterator begin_;
    iterator end_;
};

template<typename Iterator>
auto make_iterator_range(Iterator begin, Iterator end)
{
    return IteratorRange<Iterator>(begin, end);
}

template<typename Range>
auto reversed(Range& range)
{
    return make_iterator_range(std::rbegin(range), std::rend(range));
}

}  // namespace eggcpt
