#pragma once

#include <tuple>
#include <type_traits>

#include "iterator.h"

namespace eggcpt
{

template<typename Range,
         typename Integral,
         typename Iterator = decltype(std::begin(std::declval<Range>())),
         typename          = decltype(std::end(std::declval<Range>()))>
auto enumerate(Range& range, Integral start = 0)
{
    static_assert(std::is_integral_v<Integral>);

    struct iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::tuple<Integral, Iterator>*;
        using reference         = std::tuple<Integral, Iterator>&;
        using value_type        = std::tuple<Integral, Iterator>;

        bool operator!=(const iterator& other) const { return iter != other.iter; }
        void operator++() { ++index; ++iter; }
        auto operator *() { return std::tie(index, *iter); }

        Integral index;
        Iterator iter;
    };

    return make_iterator_range<iterator>(
        { start, std::begin(range) },
        { start, std::end(range)   }
    );
}

}  // namespace eggcpt
