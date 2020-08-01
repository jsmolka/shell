#pragma once

#include <array>
#include <utility>

namespace eggcpt::mpl
{

namespace detail
{

template<typename T, class Func, std::size_t ...Is>
constexpr std::array<T, sizeof...(Is)> make_array(Func&& func, std::index_sequence<Is...>)
{
    return { func(std::integral_constant<std::size_t, Is>{})... };
}

}  // namespace detail

template<typename T, std::size_t N, typename Func>
constexpr std::array<T, N> array(Func&& func)
{
    return detail::make_array<T>(std::forward<Func>(func), std::make_index_sequence<N>{});
}

}  // namespace eggcpt::mpl
