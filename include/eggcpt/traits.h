#pragma once

#include <type_traits>

namespace eggcpt
{

template<typename T, typename... Ts>
constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Ts>...>;

template<typename T, typename... Ts>
struct is_any_of : std::bool_constant<is_any_of_v<T, Ts...>> {};

template<typename T, template<typename...> typename Template>
constexpr bool is_specialization_v = false;

template<template<typename...> typename Template, typename... T>
constexpr bool is_specialization_v<Template<T...>, Template> = true;

template<typename T, template <typename...> typename Template>
struct is_specialization : std::bool_constant<is_specialization_v<T, Template>> {};

template<typename T, template<typename> typename Operation, typename = std::void_t<>>
constexpr bool is_detected_v = false;

template<typename T, template<typename> typename Operation>
constexpr bool is_detected_v<T, Operation, std::void_t<Operation<T>>> = true;

template<typename T, template<typename> typename Operation>
struct is_detected : std::bool_constant<is_detected_v<T, Operation>> {};

template<typename T, typename = std::void_t<>>
constexpr bool is_iterable_v = false;

template<typename T>
constexpr bool is_iterable_v<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>> = true;

template<typename T>
struct is_interable : std::bool_constant<is_iterable_v<T>> {};

}  // namespace eggcpt
