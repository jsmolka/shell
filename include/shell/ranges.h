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
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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
        : index(begin), end(end), step(step) {}

    RangeIterator(Integral begin, Integral end)
        : RangeIterator(begin, end, 1) {}

    RangeIterator(Integral end)
        : RangeIterator(0, end, 1) {}

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

    bool operator==(RangeIteratorSentinel) const
    {
        return step > 0 ? index >= end : index < end;
    }

    bool operator!=(RangeIteratorSentinel) const
    {
        return !(*this == RangeIteratorSentinel{});
    }

private:
    Integral index;
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
    range(Integral begin, Integral end)
{
    return { RangeIterator<Integral>(begin, end) };
}

template<typename Integral>
SentinelRange<RangeIteratorSentinel, RangeIterator<Integral>> 
    range(Integral begin, Integral end, Integral step)
{
    return { RangeIterator<Integral>(begin, end, step) };
}

template<typename Integral, typename Iterator>
class EnumerateIterator
{
public:
    static_assert(std::is_integral_v<Integral>);

    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = std::tuple<Integral, typename std::iterator_traits<Iterator>::reference>;
    using reference         = std::tuple<Integral, typename std::iterator_traits<Iterator>::reference>&;
    using pointer           = std::tuple<Integral, typename std::iterator_traits<Iterator>::reference>*;

    EnumerateIterator(Integral index, Iterator iter)
        : index(index), iter(iter) {}

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
        return iter == other.iter;
    }

    bool operator!=(const EnumerateIterator& other) const
    {
        return !(*this == other);
    }

private:
    Integral index;
    Iterator iter;
};

template<typename Range, typename Integral = std::size_t>
IteratorRange<EnumerateIterator<Integral, range_iterator_t<Range>>>
    enumerate(Range& range, Integral start = 0)
{
    using Iterator = EnumerateIterator<Integral, range_iterator_t<Range>>;

    return { Iterator(start, std::begin(range)), Iterator(start, std::end(range)) };
}

template<typename Range>
IteratorRange<range_reverse_iterator_t<Range>>
    reversed(Range& range)
{
    return { std::rbegin(range), std::rend(range) };
}

}  // namespace shell
