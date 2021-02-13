#pragma once

#include <algorithm>

#include <shell/macros.h>
#include <shell/mp.h>
#include <shell/traits.h>

namespace shell
{

#define SHELL_FORWARD_ITERATORS(Begin, End)                          \
          iterator begin()        { return       iterator(Begin); }  \
          iterator end()          { return       iterator(End);   }  \
    const_iterator begin()  const { return const_iterator(Begin); }  \
    const_iterator end()    const { return const_iterator(End);   }  \
    const_iterator cbegin() const { return const_iterator(Begin); }  \
    const_iterator cend()   const { return const_iterator(End);   }

#define SHELL_REVERSE_ITERATORS(Begin, End)                                           \
          reverse_iterator rbegin()        { return       reverse_iterator(End);   }  \
          reverse_iterator rend()          { return       reverse_iterator(Begin); }  \
    const_reverse_iterator rbegin()  const { return const_reverse_iterator(End);   }  \
    const_reverse_iterator rend()    const { return const_reverse_iterator(Begin); }  \
    const_reverse_iterator crbegin() const { return const_reverse_iterator(End);   }  \
    const_reverse_iterator crend()   const { return const_reverse_iterator(Begin); }

template<typename Iterator>
class ForwardRange
{
public:
    using iterator       = Iterator;
    using const_iterator = const iterator;

    ForwardRange(Iterator begin, Iterator end)
        : range{ begin, end } {}

    SHELL_FORWARD_ITERATORS(range.begin, range.end)

private:
    struct Range
    {
        Iterator begin;
        Iterator end;
    } range;
};

template<typename Iterator>
class BidirectionalRange
{
public:
    using iterator               = Iterator;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    BidirectionalRange(Iterator begin, Iterator end)
        : range{ begin, end } {}

    SHELL_FORWARD_ITERATORS(range.begin, range.end)
    SHELL_REVERSE_ITERATORS(range.end, range.begin)

private:
    struct Range
    {
        Iterator begin;
        Iterator end;
    } range;
};

class Sentinel {};

template<typename Iterator>
class SentinelRange
{
public:
    using iterator       = Iterator;
    using const_iterator = const iterator;
    using sentinel       = Sentinel;
    using const_sentinel = const sentinel;

    SentinelRange(Iterator begin)
        : range{ begin } {}

          iterator begin()        { return       iterator(range.begin); }
          sentinel end()          { return       sentinel();            }
    const_iterator begin()  const { return const_iterator(range.begin); }
    const_sentinel end()    const { return const_sentinel();            }
    const_iterator cbegin() const { return const_iterator(range.begin); }
    const_sentinel cend()   const { return const_sentinel();            }

private:
    struct Range
    {
        Iterator begin;
    } range;
};

template<typename T>
class PointerRange : public BidirectionalRange<T*>
{
public:
    PointerRange(T* begin, T* end)
        : BidirectionalRange<T*>(begin, end) {}

    PointerRange(T* begin, std::size_t size)
        : PointerRange(begin, begin + size) {}
};

template<typename Integral>
class RangeIterator
{
public:
    static_assert(std::is_integral_v<Integral>);

    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Integral;
    using reference         = Integral&;
    using pointer           = Integral*;

    RangeIterator(Integral begin, Integral end, Integral step)
        : index(begin), end(end), step(step) {}

    value_type operator*() const
    {
        return index;
    }

    RangeIterator& operator++()
    {
        index += step;
        return *this;
    }

    bool operator==(const RangeIterator& other) const
    {
        return index == other.index && end == other.end && step == other.step;
    }

    bool operator!=(const RangeIterator& other) const
    {
        return !(*this == other);
    }

    bool operator==(Sentinel) const
    {
        return step > 0 ? index >= end : index < end;
    }

    bool operator!=(Sentinel) const
    {
        return !(*this == Sentinel{});
    }

private:
    Integral index;
    Integral end;
    Integral step;
};

template<typename Integral>
SentinelRange<RangeIterator<Integral>>
    range(Integral end)
{
    using Iterator = RangeIterator<Integral>;

    return { Iterator(0, end, 1) };
}

template<typename Integral>
SentinelRange<RangeIterator<Integral>>
    range(Integral begin, Integral end)
{
    using Iterator = RangeIterator<Integral>;

    return { Iterator(begin, end, 1) };
}

template<typename Integral>
SentinelRange<RangeIterator<Integral>>
    range(Integral begin, Integral end, Integral step)
{
    using Iterator = RangeIterator<Integral>;

    return { Iterator(begin, end, step) };
}

template<typename Integral, typename Iterator>
class EnumerateIterator
{
public:
    static_assert(std::is_integral_v<Integral>);

    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = std::tuple<Integral, dereferenced_t<Iterator>>;
    using reference         = std::tuple<Integral, dereferenced_t<Iterator>>&;
    using pointer           = std::tuple<Integral, dereferenced_t<Iterator>>*;

    EnumerateIterator(Integral index, Iterator begin, Iterator end)
        : index(index), iter(begin), end(end) {}

    value_type operator*() const
    {
        return { index, *iter };
    }

    EnumerateIterator& operator++()
    {
        ++iter;
        ++index;
        return *this;
    }

    bool operator==(const EnumerateIterator& other) const
    {
        return index == other.index && iter == other.iter && end == other.end;
    }

    bool operator!=(const EnumerateIterator& other) const
    {
        return !(*this == other);
    }

    bool operator==(Sentinel) const
    {
        return iter == end;
    }

    bool operator!=(Sentinel) const
    {
        return !(*this == Sentinel{});
    }

private:
    Integral index;
    Iterator iter;
    Iterator end;
};

template<typename Range, typename Integral = std::size_t>
SentinelRange<EnumerateIterator<Integral, range_iterator_t<Range>>>
    enumerate(Range& range, Integral start = 0)
{
    using Iterator = EnumerateIterator<Integral, range_iterator_t<Range>>;

    return { Iterator(start, std::begin(range), std::end(range)) };
}

template<typename... Iterators>
class ZipIterator
{
public:
    static_assert(sizeof...(Iterators) > 0);

    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = std::tuple<dereferenced_t<Iterators>...>;
    using reference         = std::tuple<dereferenced_t<Iterators>...>&;
    using pointer           = std::tuple<dereferenced_t<Iterators>...>*;

    ZipIterator(Iterators... begins, mp::head_t<Iterators...> end)
        : iters(begins...), end(end) {}

    value_type operator*() const
    {
        auto dereference = [](const Iterators&... iters) -> value_type { return { *iters... }; } ;

        return std::apply(dereference, iters);
    }

    ZipIterator& operator++()
    {
        auto advance = [](Iterators&... iters) { (++iters, ...); };

        std::apply(advance, iters);

        return *this;
    }

    bool operator==(const ZipIterator& other) const
    {
        return iters == other.iters && end == other.end;
    }

    bool operator!=(const ZipIterator& other) const
    {
        return !(*this == other);
    }

    bool operator==(Sentinel) const
    {
        return std::get<0>(iters) == end;
    }

    bool operator!=(Sentinel) const
    {
        return !(*this == Sentinel{});
    }

private:
    std::tuple<Iterators...> iters;
    mp::head_t<Iterators...> end;
};

template<typename... Ranges>
SentinelRange<ZipIterator<range_iterator_t<Ranges>...>>
    zip(Ranges&... ranges)
{
    using Iterator = ZipIterator<range_iterator_t<Ranges>...>;

    return { Iterator(std::begin(ranges)..., std::end(mp::head_element(ranges...))) };
}

template<typename Range>
ForwardRange<range_reverse_iterator_t<Range>>
    reversed(Range& range)
{
    return { std::rbegin(range), std::rend(range) };
}

template<typename Range, typename Function>
void foreach(Range& range, Function func)
{
    std::for_each(std::begin(range), std::end(range), func);
}

template<typename Range, typename T>
bool contains(const Range& range, const T& value)
{
    return std::find(std::begin(range), std::end(range), value) != std::end(range);
}

}  // namespace shell
