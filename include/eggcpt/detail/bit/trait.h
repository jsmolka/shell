#pragma once

#include <limits>
#include <type_traits>

namespace eggcpt::bit
{

template<typename T>
struct bits : std::integral_constant<unsigned, CHAR_BIT * sizeof(T)> {};

template<typename T>
constexpr auto bits_v = bits<T>::value;

}  // namespace eggcpt::bit
