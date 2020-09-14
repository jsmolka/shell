#pragma once

#include <iterator>
#include <type_traits>

namespace eggcpt
{

template<typename T, typename... Ts>
inline constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Ts>...>;

template<typename T, typename... Ts>
struct is_any_of : std::bool_constant<is_any_of_v<T, Ts...>> {};

template<typename T, template<typename> typename Func, typename = std::void_t<>>
inline constexpr bool is_detected_v = false;

template<typename T, template<typename> typename Func>
inline constexpr bool is_detected_v<T, Func, std::void_t<Func<T>>> = true;

template<typename T, template<typename> typename Func>
struct is_detected : std::bool_constant<is_detected_v<T, Func>> {};

template<typename Range>
struct range_traits
{
    using iterator               = typename Range::iterator;
    using const_iterator         = typename Range::const_iterator;
    using reverse_iterator       = typename Range::reverse_iterator;
    using const_reverse_iterator = typename Range::const_reverse_iterator;
};

template<typename Range, std::size_t N>
struct range_traits<Range[N]>
{
    using iterator               = Range*;
    using const_iterator         = const Range*;
    using reverse_iterator       = Range*;
    using const_reverse_iterator = const Range*;
};

template<typename Range>
struct range_iterator
{
    using type = std::conditional_t<
        std::is_const_v<Range>,
        typename range_traits<Range>::const_iterator,
        typename range_traits<Range>::iterator>;
};

template<typename Range>
using range_iterator_t = typename range_iterator<Range>::type;

template<typename Range>
struct range_reverse_iterator
{
    using type = std::conditional_t<
        std::is_const_v<Range>,
        typename range_traits<Range>::const_reverse_iterator,
        typename range_traits<Range>::reverse_iterator>;
};

template<typename Range>
using range_reverse_iterator_t = typename range_reverse_iterator<Range>::type;

template<typename Range>
struct range_iterator_traits : std::iterator_traits<range_iterator_t<Range>> {};

template<typename Range>
struct range_value
{
    using type = typename range_iterator_traits<Range>::value_type;
};

template<typename Range>
using range_value_t = typename range_value<Range>::type;

}  // namespace eggcpt
