#pragma once

#include <tuple>
#include <type_traits>

#include "iterator.h"

namespace eggcpt
{

template<typename Integral, typename Iterator>
struct EnumerateIterator
{
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = std::tuple<Integral, Iterator>;
    using reference         = std::tuple<Integral, Iterator>&;
    using pointer           = std::tuple<Integral, Iterator>*;

    bool operator!=(const EnumerateIterator& other) const { return iter != other.iter; }
    void operator++() { ++index; ++iter; }
    auto operator*() const { return std::tie(index, *iter); }

    Integral index;
    Iterator iter;
};

template<typename Range, typename Integral = int>
auto enumerate(Range& range, Integral start = 0)
{
    static_assert(std::is_integral_v<Integral>);
    
    using Iterator = decltype(std::begin(std::declval<Range&>()));

    return make_iterator_range(
        EnumerateIterator<Integral, Iterator>{ start, std::begin(range) },
        EnumerateIterator<Integral, Iterator>{ start, std::end(range) }
    );
}

}  // namespace eggcpt
