#pragma once

#include <tuple>
#include <utility>

namespace shell::mp
{

template<std::size_t Index, typename... Ts>
struct nth
{
    using type = std::tuple_element_t<Index, std::tuple<Ts...>>;
};

template<std::size_t Index, typename... Ts>
using nth_t = typename nth<Index, Ts...>::type;

template<std::size_t Index, typename... Args>
nth_t<Index, Args...> nth_element(Args&&... args)
{
    return std::get<Index>(std::forward_as_tuple(std::forward<Args>(args)...));
}

template<typename... Ts>
struct head : nth<0, Ts...> {};

template<typename... Ts>
using head_t = typename head<Ts...>::type;

template<typename... Args>
head_t<Args...> head_element(Args&&... args)
{
    return nth_element<0>(args...);
}

template<typename... Ts>
struct tail : nth<sizeof...(Ts) - 1, Ts...> {};

template<typename... Ts>
using tail_t = typename tail<Ts...>::type;

template<typename... Args>
tail_t<Args...> tail_element(Args&&... args)
{
    return nth_element<sizeof...(Args) - 1>(args...);
}

}  // namespace shell::mp
