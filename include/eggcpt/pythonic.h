#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include "iterator.h"

namespace eggcpt
{

template<typename Integral, typename Iterator>
class EnumerateIterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = std::tuple<const Integral&, typename std::iterator_traits<Iterator>::reference>;
    using reference         = std::tuple<const Integral&, typename std::iterator_traits<Iterator>::reference>&;
    using pointer           = std::tuple<const Integral&, typename std::iterator_traits<Iterator>::reference>*;

    EnumerateIterator(Integral index, Iterator iter)
        : index(index), iter(iter) {}

    value_type operator*() const
    {
        return std::tie(index, *iter);
    }
    
    EnumerateIterator& operator++()
    {
        ++iter;
        ++index;
        return *this;
    }

    bool operator==(const EnumerateIterator& other) const { return iter == other.iter; }
    bool operator!=(const EnumerateIterator& other) const { return iter != other.iter; }

private:
    Integral index;
    Iterator iter;
};

template<typename Range, typename Integral = int>
auto enumerate(Range& range, Integral start = 0)
{
    static_assert(std::is_integral_v<Integral>);
    
    using Iterator = decltype(std::begin(std::declval<Range&>()));

    return make_iterator_range(
        EnumerateIterator<Integral, Iterator>(start, std::begin(range)),
        EnumerateIterator<Integral, Iterator>(start, std::end(range))
    );
}

}  // namespace eggcpt
