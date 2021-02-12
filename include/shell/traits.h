#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

namespace shell
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

template<typename T, template<typename...> typename Template>
inline constexpr bool is_specialization_v = false;

template<template<typename...> typename Template, typename... Ts>
inline constexpr bool is_specialization_v<Template<Ts...>, Template> = true;

template<typename T, template<typename...> typename Template>
struct is_specialization : std::bool_constant<is_specialization_v<T, Template>> {};

template<typename T>
struct unqualified
{
    using type = std::remove_cv_t<std::remove_pointer_t<std::decay_t<T>>>;
};

template<typename T>
using unqualified_t = typename unqualified<T>::type;

template<typename T>
struct dereferenced
{
    using type = decltype(*std::declval<T>());
};

template<typename T>
using dereferenced_t = typename dereferenced<T>::type;

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
    using const_iterator         = iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
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
struct range_value
{
    using type = typename std::iterator_traits<range_iterator_t<Range>>::value_type;
};

template<typename Range>
using range_value_t = typename range_value<Range>::type;

template<typename Range>
struct range_reference
{
    using type = typename std::iterator_traits<range_iterator_t<Range>>::reference;
};

template<typename Range>
using range_reference_t = typename range_reference<Range>::type;

template<typename Range>
struct range_pointer
{
    using type = typename std::iterator_traits<range_iterator_t<Range>>::pointer;
};

template<typename Range>
using range_pointer_t = typename range_pointer<Range>::type;

}  // namespace shell
