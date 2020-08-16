#pragma once

namespace eggcpt
{

template<typename ForwardIterator>
class IteratorRange
{
public:
    using iterator = ForwardIterator;
    using const_iterator = const ForwardIterator;

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

template<typename ForwardIterator>
auto make_iterator_range(ForwardIterator begin, ForwardIterator end)
{
    return IteratorRange<ForwardIterator>(begin, end);
}

template<typename Range>
auto reversed(Range& range)
{
    return make_iterator_range(std::rbegin(range), std::rend(range));
}

}  // namespace eggcpt
