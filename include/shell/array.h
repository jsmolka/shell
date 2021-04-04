#pragma once

#include <array>

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

}  // namespace detail

template<typename T, std::size_t kSize, std::size_t... kSizes>
using array = typename detail::array<T, kSize, kSizes...>::type;

}  // namespace shell
