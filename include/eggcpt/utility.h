#pragma once

#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "iterator.h"
#include "primitives.h"
#include "traits.h"

namespace eggcpt
{

template<typename T, typename... Args>
void reconstruct(T& instance, Args&&... args)
{
    static_assert(std::is_constructible_v<T, Args...>);

    instance.~T();
    new(&instance)T(std::forward<Args>(args)...);
}

template<typename Primitive, typename Char>
std::optional<Primitive> parse_primitive(const std::basic_string<Char>& data)
{
    static_assert(is_any_of_v<Char, char, wchar_t>);
    static_assert(is_any_of_v<Primitive, s32, u32, s64, u64, f32, f64>);

    try
    {
        if constexpr (std::is_same_v<Primitive, s32>) return std::stoi(data);
        if constexpr (std::is_same_v<Primitive, u32>) return std::stoul(data);
        if constexpr (std::is_same_v<Primitive, s64>) return std::stoll(data);
        if constexpr (std::is_same_v<Primitive, u64>) return std::stoull(data);
        if constexpr (std::is_same_v<Primitive, f32>) return std::stof(data);
        if constexpr (std::is_same_v<Primitive, f64>) return std::stod(data);
    } 
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
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

template<typename Range, typename Integral = int>
auto enumerate(Range& range, Integral start = 0)
{
    using Iterator = decltype(std::begin(std::declval<Range&>()));

    return make_iterator_range(
        EnumerateIterator<Integral, Iterator>(start, std::begin(range)),
        EnumerateIterator<Integral, Iterator>(start, std::end(range))
    );
}

}  // namespace eggcpt
