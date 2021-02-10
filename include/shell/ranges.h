#pragma once

#include <iterator>

#include <shell/macros.h>
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
class IteratorRange
{
public:
    using iterator       = Iterator;
    using const_iterator = const iterator;

    IteratorRange(Iterator begin, Iterator end)
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
class BidirectionalIteratorRange
{
public:
    using iterator               = Iterator;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<Iterator>;
    using const_reverse_iterator = std::reverse_iterator<const Iterator>;

    BidirectionalIteratorRange(Iterator begin, Iterator end)
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

template<typename Sentinel, typename Iterator>
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
          sentinel end()          { return       sentinel{};            }
    const_iterator begin()  const { return const_iterator(range.begin); }
    const_sentinel end()    const { return const_sentinel{};            }
    const_iterator cbegin() const { return const_iterator(range.begin); }
    const_sentinel cend()   const { return const_sentinel{};            }

private:
    struct Range
    {
        Iterator begin;
    } range;
};

template<typename T>
class PointerRange : public BidirectionalIteratorRange<T*>
{
public:
    PointerRange(T* begin, T* end)
        : BidirectionalIteratorRange<T*>(begin, end) {}

    PointerRange(T* begin, std::size_t size)
        : PointerRange(begin, begin + size) {}
};

class RangeIteratorSentinel {};

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
        : begin(begin), end(end), step(step) {}

    RangeIterator(Integral begin, Integral end)
        : RangeIterator(begin, end, 1) {}

    RangeIterator(Integral end)
        : RangeIterator(0, end, 1) {}

    value_type operator*() const
    {
        return begin;
    }

    RangeIterator& operator++()
    {
        begin += step;
        return *this;
    }

    bool operator==(const RangeIterator& other) const
    {
        return begin == other.begin && end == other.end && step == other.step;
    }

    bool operator!=(const RangeIterator& other) const
    {
        return !(*this == other);
    }

    bool operator==(RangeIteratorSentinel) const
    {
        return step > 0 ? begin >= end : begin >= end;
    }

    bool operator!=(RangeIteratorSentinel) const
    {
        return !(*this == RangeIteratorSentinel{});
    }

private:
    Integral begin;
    Integral end;
    Integral step;
};

template<typename Integral>
SentinelRange<RangeIteratorSentinel, RangeIterator<Integral>> 
    range(Integral end)
{
    return { RangeIterator<Integral>(end) };
}

template<typename Integral>
SentinelRange<RangeIteratorSentinel, RangeIterator<Integral>> 
    range(Integral start, Integral end)
{
    return { RangeIterator<Integral>(start, end) };
}

template<typename Integral>
SentinelRange<RangeIteratorSentinel, RangeIterator<Integral>> 
    range(Integral start, Integral end, Integral step)
{
    return { RangeIterator<Integral>(start, end, step) };
}

template<typename Range>
IteratorRange<range_reverse_iterator_t<Range>>
    reversed(Range& range)
{
    return IteratorRange(std::rbegin(range), std::rend(range));
}

}  // namespace shell
