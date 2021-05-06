#pragma once

#include <algorithm>

#include <shell/int.h>
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

template<typename Iterator>
class ForwardRange
{
public:
    using iterator       = Iterator;
    using const_iterator = const iterator;

    ForwardRange(Iterator begin, Iterator end)
        : _begin(begin), _end(end) {}

    SHELL_FORWARD_ITERATORS(_begin, _end)

private:
    const Iterator _begin;
    const Iterator _end;
};

#define SHELL_REVERSE_ITERATORS(Begin, End)                                           \
          reverse_iterator rbegin()        { return       reverse_iterator(Begin); }  \
          reverse_iterator rend()          { return       reverse_iterator(End);   }  \
    const_reverse_iterator rbegin()  const { return const_reverse_iterator(Begin); }  \
    const_reverse_iterator rend()    const { return const_reverse_iterator(End);   }  \
    const_reverse_iterator crbegin() const { return const_reverse_iterator(Begin); }  \
    const_reverse_iterator crend()   const { return const_reverse_iterator(End);   }

template<typename Iterator>
class BidirectionalRange
{
public:
    using iterator               = Iterator;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    BidirectionalRange(Iterator begin, Iterator end)
        : _begin(begin), _end(end) {}

    SHELL_FORWARD_ITERATORS(_begin, _end)
    SHELL_REVERSE_ITERATORS(_end, _begin)

private:
    const Iterator _begin;
    const Iterator _end;
};

#define SHELL_SENTINEL_ITERATORS(Begin)                              \
          iterator begin()        { return       iterator(Begin); }  \
          sentinel end()          { return       sentinel();      }  \
    const_iterator begin()  const { return const_iterator(Begin); }  \
    const_sentinel end()    const { return const_sentinel();      }  \
    const_iterator cbegin() const { return const_iterator(Begin); }  \
    const_sentinel cend()   const { return const_sentinel();      }

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
        : _begin(begin) {}

    SHELL_SENTINEL_ITERATORS(_begin)

private:
    const Iterator _begin;
};

template<typename T, typename Byte = u8>
class ByteRange : public BidirectionalRange<Byte*>
{
public:
    static_assert(sizeof(Byte) == 1);

    ByteRange(T* value)
        : BidirectionalRange<Byte*>(
            reinterpret_cast<Byte*>(value),
            reinterpret_cast<Byte*>(value) + sizeof(T)) {}

    ByteRange(T& value)
        : ByteRange(&value) {}
};

template<typename Integral, typename Iterator>
class EnumerateIterator
{
public:
    static_assert(std::is_integral_v<Integral>);

    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = std::tuple<Integral, dereferenced_t<Iterator>>;
    using reference         = value_type&;
    using pointer           = value_type*;

    EnumerateIterator(Integral index, Iterator begin, Iterator end)
        : _index(index), _iter(begin), _end(end) {}

    value_type operator*() const
    {
        return { _index, *_iter };
    }

    EnumerateIterator& operator++()
    {
        ++_iter;
        ++_index;
        return *this;
    }

    bool operator==(const EnumerateIterator& other) const
    {
        return _index == other._index && _iter == other._iter && _end == other._end;
    }

    bool operator!=(const EnumerateIterator& other) const
    {
        return !(*this == other);
    }

    bool operator==(Sentinel) const
    {
        return _iter == _end;
    }

    bool operator!=(Sentinel) const
    {
        return !(*this == Sentinel{});
    }

private:
    Integral _index;
    Iterator _iter;
    const Iterator _end;
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
    using reference         = value_type&;
    using pointer           = value_type*;

    ZipIterator(Iterators... begins, mp::first_t<Iterators...> end)
        : _iters(begins...), _end(end) {}

    value_type operator*() const
    {
        auto dereference = [](const Iterators&... iters) -> value_type { return { *iters... }; } ;

        return std::apply(dereference, _iters);
    }

    ZipIterator& operator++()
    {
        auto advance = [](Iterators&... iters) { (++iters, ...); };

        std::apply(advance, _iters);

        return *this;
    }

    bool operator==(const ZipIterator& other) const
    {
        return _iters == other._iters && _end == other._end;
    }

    bool operator!=(const ZipIterator& other) const
    {
        return !(*this == other);
    }

    bool operator==(Sentinel) const
    {
        return std::get<0>(_iters) == _end;
    }

    bool operator!=(Sentinel) const
    {
        return !(*this == Sentinel{});
    }

private:
    std::tuple<Iterators...> _iters;
    const mp::first_t<Iterators...> _end;
};

template<typename... Ranges>
SentinelRange<ZipIterator<range_iterator_t<Ranges>...>> zip(Ranges&... ranges)
{
    using Iterator = ZipIterator<range_iterator_t<Ranges>...>;

    return { Iterator(std::begin(ranges)..., std::end(mp::first_element(ranges...))) };
}

template<typename Range> 
ForwardRange<range_reverse_iterator_t<Range>> reversed(Range& range)
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
