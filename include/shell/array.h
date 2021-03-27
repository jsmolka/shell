#pragma once

#include <array>

namespace shell
{

template<typename T, std::size_t I, std::size_t... Is>
struct array
{
    using type = std::array<typename array<T, Is...>::type, I>;
};

template<typename T, std::size_t I>
struct array<T, I>
{
    using type = std::array<T, I>;
};

template<typename T, std::size_t I, std::size_t... Is>
using array_t = typename array<T, I, Is...>::type;

}  // namespace shell
