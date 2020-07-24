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

template<typename T, template<typename> typename Operation, typename Void = std::void_t<>>
struct detect : std::false_type {};

template<typename T, template<typename> typename Operation>
struct detect<T, Operation, std::void_t<Operation<T>>> : std::true_type {};

template<typename T, template<typename> typename Operation, typename Void = std::void_t<>>
constexpr bool detect_v = detect<T, Operation, Void>::value;

}  // namespace eggcpt
