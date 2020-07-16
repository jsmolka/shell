#pragma once

#include <cstddef>
#include <type_traits>

namespace eggcpt::traits
{

template<typename T>
constexpr bool is_integer_v = std::is_integral_v<T>;

template<typename T>
struct is_integer : std::bool_constant<is_integer_v<T>> {};

template<typename T, typename... Ts>
constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Ts>...>;

template<typename T, typename... Ts>
struct is_any_of : std::bool_constant<is_any_of_v<T, Ts...>> {};

template<typename T, std::size_t N>
constexpr bool is_size_least_v = sizeof(T) >= N;

template<typename T, std::size_t N>
struct is_size_least : std::bool_constant<is_size_least_v<T, N>> {};

template<typename T, std::size_t N>
constexpr bool is_size_most_v = sizeof(T) <= N;

template<typename T, std::size_t N>
struct is_size_most : std::bool_constant<is_size_most_v<T, N>> {};

}  // namespace eggcpt::traits
