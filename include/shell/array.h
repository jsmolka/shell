#pragma once

#include <array>
#include <type_traits>
#include <utility>

namespace shell
{

namespace detail
{

template<typename T, std::size_t kSize, std::size_t... kSizes>
struct array
{
    using type = std::array<typename array<T, kSizes...>::type, kSize>;
};

template<typename T, std::size_t kSize>
struct array<T, kSize>
{
    using type = std::array<T, kSize>;
};

template<typename T, class Func, std::size_t ...kIs>
constexpr std::array<T, sizeof...(kIs)> makeArrayImpl(Func&& func, std::index_sequence<kIs...>)
{
    return { func(std::integral_constant<std::size_t, kIs>{})... };
}

}  // namespace detail

template<typename T, std::size_t kSize, std::size_t... kSizes>
using array = typename detail::array<T, kSize, kSizes...>::type;

template<typename T, std::size_t kSize, class Func>
constexpr std::array<T, kSize> makeArray(Func&& func)
{
    return detail::makeArrayImpl<T>(std::forward<Func>(func), std::make_index_sequence<kSize>{});
}

}  // namespace shell
