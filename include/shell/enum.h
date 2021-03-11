#pragma once

#include <shell/traits.h>

namespace shell::detail
{

template<typename T>
inline constexpr bool is_bitwise_type_v = std::is_enum_v<T> || std::is_integral_v<T>;

template<typename T>
struct is_bitwise_type : std::bool_constant<is_bitwise_type_v<T>> {};

template<typename T, bool = std::is_enum_v<T>>
struct bitwise_type
{
    static_assert(is_bitwise_type_v<T>);

    using type = std::underlying_type_t<T>;
};

template<typename T>
struct bitwise_type<T, false>
{
    static_assert(is_bitwise_type_v<T>);

    using type = T;
};

template<typename T>
using bitwise_type_t = typename bitwise_type<T>::type;

template<typename T, typename U>
struct bitwise
{
    static_assert(is_bitwise_type_v<T> && is_bitwise_type_v<U>);

    using type = std::common_type_t<bitwise_type_t<T>, bitwise_type_t<U>>;
};

template<typename T, typename U>
using bitwise_t = typename bitwise<T, U>::type;

template<typename T, typename U>
inline constexpr bool is_bitwise_enabled_v =
    (is_scoped_enum_v<T> || is_scoped_enum_v<U>) &&
    is_bitwise_type_v<T> && is_bitwise_type_v<U>;

template<typename T, typename U>
struct is_bitwise_enabled : std::bool_constant<is_bitwise_enabled_v<T, U>> {};

template<typename T, typename U>
inline constexpr bool is_bitwise_equals_enabled_v = std::is_integral_v<T> && is_scoped_enum_v<U>;

template<typename T, typename U>
struct is_bitwise_equals_enabled : std::bool_constant<is_bitwise_equals_enabled_v<T, U>> {};

}  // namespace shell::detail

template<
    typename T,
    typename U,
    typename R = shell::detail::bitwise_t<T, U>,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr R operator&(const T& t, const U& u)
{
    return static_cast<R>(t) & static_cast<R>(u);
}

template<
    typename T,
    typename U,
    typename R = shell::detail::bitwise_t<T, U>,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr R operator|(const T& t, const U& u)
{
    return static_cast<R>(t) | static_cast<R>(u);
}

template<
    typename T,
    typename U,
    typename R = shell::detail::bitwise_t<T, U>,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr R operator^(const T& t, const U& u)
{
    return static_cast<R>(t) ^ static_cast<R>(u);
}

template<
    typename T, 
    typename R = shell::detail::bitwise_t<T, T>,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, T>>>
constexpr R operator~(const T& t)
{
    return ~static_cast<R>(t);
}

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_equals_enabled_v<T, U>>>
constexpr T& operator&=(T& t, const U& u)
{
    return t = t & static_cast<T>(u);
}

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_equals_enabled_v<T, U>>>
constexpr T& operator|=(T& t, const U& u)
{
    return t = t | static_cast<T>(u);
}

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_equals_enabled_v<T, U>>>
constexpr T& operator^=(T& t, const U& u)
{
    return t = t ^ static_cast<T>(u);
}
