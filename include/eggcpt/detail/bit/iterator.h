#pragma once

#include <iterator>
#include <type_traits>

#include <eggcpt/detail/bit/intrin.h>
#include <eggcpt/detail/utility/iterator.h>

namespace eggcpt::bit
{

template<typename T>
class bit_iterator
{
    static_assert(std::is_integral_v<T>);

public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using value_type = T;

    bit_iterator(T value)
        : value(value) {}

    unsigned operator*() const
    {
        return ctz(value);
    }

    bit_iterator& operator++()
    {
        value &= value - 1;
        return *this;
    }

    bool operator==(bit_iterator other) const { return value == other.value; }
    bool operator!=(bit_iterator other) const { return value != other.value; }

private:
    T value;
};

template<typename T>
auto iterate(T value)
{
    static_assert(std::is_integral_v<T>);

    return utility::make_iterator_range<bit_iterator<T>>(value, 0);
}

}  // namespace eggcpt::bit
