#pragma once

#include <tuple>
#include <utility>

namespace shell::mp
{

template<std::size_t kIndex, typename... Ts>
struct nth
{
    using type = std::tuple_element_t<kIndex, std::tuple<Ts...>>;
};

template<std::size_t kIndex, typename... Ts>
using nth_t = typename nth<kIndex, Ts...>::type;

template<std::size_t kIndex, typename... Args>
nth_t<kIndex, Args...> nth_element(Args&&... args)
{
    return std::get<kIndex>(std::forward_as_tuple(std::forward<Args>(args)...));
}

template<typename... Ts>
struct first : nth<0, Ts...> {};

template<typename... Ts>
using first_t = typename first<Ts...>::type;

template<typename... Args>
first_t<Args...> first_element(Args&&... args)
{
    return nth_element<0>(std::forward<Args>(args)...);
}

template<typename... Ts>
struct last : nth<sizeof...(Ts) - 1, Ts...> {};

template<typename... Ts>
using last_t = typename last<Ts...>::type;

template<typename... Args>
last_t<Args...> last_element(Args&&... args)
{
    return nth_element<sizeof...(Args) - 1>(std::forward<Args>(args)...);
}

}  // namespace shell::mp
