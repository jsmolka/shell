#pragma once

#include <tuple>
#include <utility>

#include <shell/iterator.h>
#include <shell/traits.h>

namespace shell
{

namespace utility
{

template<typename T, typename... Args>
void reconstruct(T& instance, Args&&... args)
{
    static_assert(std::is_constructible_v<T, Args...>);

    instance.~T();
    new(&instance)T(std::forward<Args>(args)...);
}

template<typename Integral, typename Iterator>
class EnumerateIterator
{
    static_assert(std::is_integral_v<Integral>);

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

template<typename Range, typename Integral = std::size_t>
IteratorRange<EnumerateIterator<Integral, range_iterator_t<Range>>>
    enumerate(Range& range, Integral start = 0)
{
    using Iterator = range_iterator_t<Range>;

    return makeIteratorRange(
        EnumerateIterator<Integral, Iterator>(start, std::begin(range)),
        EnumerateIterator<Integral, Iterator>(start, std::end(range)));
}

}  // namespace utility

using utility::enumerate;
using utility::EnumerateIterator;
using utility::reconstruct;

}  // namespace shell
