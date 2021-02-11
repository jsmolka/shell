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

}  // namespace shell::mp
