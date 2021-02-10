#pragma once

#include <iterator>
#include <utility>

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

    SHELL_FORWARD_ITERATORS(std::get<0>(range), std::get<1>(range))

private:
    std::tuple<Iterator, Iterator> range;
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

    SHELL_FORWARD_ITERATORS(std::get<0>(range), std::get<1>(range))
    SHELL_REVERSE_ITERATORS(std::get<1>(range), std::get<0>(range))

private:
    std::tuple<Iterator, Iterator> range;
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

template<typename Range>
IteratorRange<range_reverse_iterator_t<Range>> reversed(Range& range)
{
    return IteratorRange(std::rbegin(range), std::rend(range));
}

}  // namespace shell
