#pragma once

#include <iterator>

#include <shell/traits.h>

namespace shell
{

#define SHELL_FORWARD_ITERATORS(begin_, end_)                         \
          iterator begin()        { return       iterator(begin_); }  \
          iterator end()          { return       iterator(end_);   }  \
    const_iterator begin()  const { return const_iterator(begin_); }  \
    const_iterator end()    const { return const_iterator(end_);   }  \
    const_iterator cbegin() const { return const_iterator(begin_); }  \
    const_iterator cend()   const { return const_iterator(end_);   }

#define SHELL_REVERSE_ITERATORS(begin_, end_)                                          \
          reverse_iterator rbegin()        { return       reverse_iterator(end_);   }  \
          reverse_iterator rend()          { return       reverse_iterator(begin_); }  \
    const_reverse_iterator rbegin()  const { return const_reverse_iterator(end_);   }  \
    const_reverse_iterator rend()    const { return const_reverse_iterator(begin_); }  \
    const_reverse_iterator crbegin() const { return const_reverse_iterator(end_);   }  \
    const_reverse_iterator crend()   const { return const_reverse_iterator(begin_); }

template<typename Iterator>
class IteratorRange
{
public:
    using iterator       = Iterator;
    using const_iterator = const iterator;

    IteratorRange(Iterator begin, Iterator end)
        : begin_(begin), end_(end) {}

    SHELL_FORWARD_ITERATORS(begin_, end_)

private:
    Iterator begin_;
    Iterator end_;
};

template<typename T>
class PointerRange
{
public:
    using iterator               = T*;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    PointerRange(T* begin, T* end)
        : begin_(begin), end_(end) {}

    PointerRange(T* begin, std::size_t size)
        : begin_(begin), end_(begin + size) {}

    SHELL_FORWARD_ITERATORS(begin_, end_)
    SHELL_REVERSE_ITERATORS(begin_, end_)

private:
    T* begin_;
    T* end_;
};

template<typename Range>
IteratorRange<range_reverse_iterator_t<Range>> reversed(Range& range)
{
    return IteratorRange(std::rbegin(range), std::rend(range));
}

}  // namespace shell
