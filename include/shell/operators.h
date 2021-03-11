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
    using type = std::underlying_type_t<T>;
};

template<typename T>
struct bitwise_type<T, false>
{
    using type = T;
};

template<typename T>
using bitwise_type_t = typename bitwise_type<T>::type;

template<typename T, typename U>
inline constexpr bool is_bitwise_enabled_v =
    (is_scoped_enum_v<T> || is_scoped_enum_v<U>) &&
    is_bitwise_type_v<T> && is_bitwise_type_v<U>;

template<typename T, typename U>
struct is_bitwise_enabled : std::bool_constant<is_bitwise_enabled_v<T, U>> {};

}  // namespace shell::detail

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr T operator&(const T& t, const U& u)
{
    using Integral = std::common_type_t<
        shell::detail::bitwise_type_t<T>,
        shell::detail::bitwise_type_t<U>>;

    return static_cast<T>(static_cast<Integral>(t) & static_cast<Integral>(u));
}

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr T operator|(const T& t, const U& u)
{
    using Integral = std::common_type_t<
        shell::detail::bitwise_type_t<T>,
        shell::detail::bitwise_type_t<U>>;

    return static_cast<T>(static_cast<Integral>(t) | static_cast<Integral>(u));
}

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr T operator^(const T& t, const U& u)
{
    using Integral = std::common_type_t<
        shell::detail::bitwise_type_t<T>,
        shell::detail::bitwise_type_t<U>>;

    return static_cast<T>(static_cast<Integral>(t) ^ static_cast<Integral>(u));
}

template<
    typename T, 
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, T>>>
constexpr T operator~(const T& t)
{
    using Integral = shell::detail::bitwise_type_t<T>;

    return static_cast<T>(~static_cast<Integral>(t));
}

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr T operator>>(const T& t, const U& u)
{
    using Integral = std::common_type_t<
        shell::detail::bitwise_type_t<T>,
        shell::detail::bitwise_type_t<U>>;

    return static_cast<T>(static_cast<Integral>(t) >> static_cast<Integral>(u));
}

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr T operator<<(const T& t, const U& u)
{
    using Integral = std::common_type_t<
        shell::detail::bitwise_type_t<T>,
        shell::detail::bitwise_type_t<U>>;

    return static_cast<T>(static_cast<Integral>(t) << static_cast<Integral>(u));
}

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr T& operator&=(T& t, const U& u)
{
    return t = t & u;
}

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr T& operator|=(T& t, const U& u)
{
    return t = t | u;
}

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr T& operator^=(T& t, const U& u)
{
    return t = t ^ u;
}

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr T& operator<<=(T& t, const U& u)
{
    return t = t << u;
}

template<
    typename T,
    typename U,
    typename = std::enable_if_t<shell::detail::is_bitwise_enabled_v<T, U>>>
constexpr T& operator>>=(T& t, const U& u)
{
    return t = t >> u;
}
