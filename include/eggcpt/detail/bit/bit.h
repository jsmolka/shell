#pragma once

#include <eggcpt/detail/bit/trait.h>

namespace eggcpt::bit
{

template<unsigned Index, unsigned Size, typename T>
constexpr T subset(T value)
{
    static_assert(std::is_integral_v<T>);
    static_assert(Index + Size <= bits_v<T>);

    constexpr T kMask = ~static_cast<T>(0) >> (bits_v<T> - Size);

    return (value >> Index) & kMask;
}

template<unsigned Size, typename T>
constexpr T signEx(T value)
{
    static_assert(std::is_integral_v<T>);
    static_assert(Size > 0 && Size <= bits_v<T>);

    constexpr T kAmount = bits_v<T> - Size;

    return static_cast<std::make_signed_t<T>>(value << kAmount) >> kAmount;
}

template<typename T>
constexpr T sar(T value, unsigned amount)
{
    static_assert(std::is_integral_v<T>);

    return static_cast<std::make_signed_t<T>>(value) >> amount;
}

template<typename T>
constexpr T shr(T value, unsigned amount)
{
    static_assert(std::is_integral_v<T>);

    return static_cast<std::make_unsigned_t<T>>(value) >> amount;
}

}  // namespace eggcpt::bit
