#pragma once

#include <iterator>
#include <tuple>

#include "iterator.h"

namespace eggcpt
{

namespace pythonic
{

template<typename T,
         typename Integral,
         typename Iterator = decltype(std::begin(std::declval<T>())),
         typename          = decltype(std::end(std::declval<T>()))>
auto enumerate(T&& iterable, Integral start = 0)
{
    struct iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::tuple<Integral, Iterator>*;
        using reference         = std::tuple<Integral, Iterator>&;
        using value_type        = std::tuple<Integral, Iterator>;

        bool operator!=(const iterator& other) const { return iter != other.iter; }
        void operator++() { ++index; ++iter; }
        auto operator *() const { return std::tie(index, *iter); }

        Integral index;
        Iterator iter;
    };

    return make_iterator_range<iterator>(
        { start, std::begin(iterable) },
        { start, std::end(iterable)   }
    );
}

}  // namespace pythonic

using pythonic::enumerate;

}  // namespace eggcpt
