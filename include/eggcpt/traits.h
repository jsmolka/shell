#pragma once

#include <type_traits>

namespace eggcpt
{

template<typename T, typename... Ts>
constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Ts>...>;

template<typename T, typename... Ts>
struct is_any_of : std::bool_constant<is_any_of_v<T, Ts...>> {};

template<typename T, template<typename> typename Func, typename = std::void_t<>>
constexpr bool is_detected_v = false;

template<typename T, template<typename> typename Func>
constexpr bool is_detected_v<T, Func, std::void_t<Func<T>>> = true;

template<typename T, template<typename> typename Func>
struct is_detected : std::bool_constant<is_detected_v<T, Func>> {};

}  // namespace eggcpt
