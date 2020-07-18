#pragma once

#include <eggcpt/detail/bit/storage.h>
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
constexpr T sign_ex(T value)
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

template<typename T>
constexpr auto isolate_nibbles(T value)
{
    static_assert(std::is_integral_v<T>);
    static_assert(sizeof(T) <= 4);

    auto x = static_cast<storage_type_t<2 * sizeof(T)>>(
        static_cast<std::make_unsigned_t<T>>(value));

    if constexpr (sizeof(T) >= 4) x = ((x & 0x0000'0000'FFFF'0000) << 16) | (x & 0x0000'0000'0000'FFFF);
    if constexpr (sizeof(T) >= 2) x = ((x & 0xFF00'FF00'FF00'FF00) <<  8) | (x & 0x00FF'00FF'00FF'00FF);
    if constexpr (sizeof(T) >= 1) x = ((x & 0xF0F0'F0F0'F0F0'F0F0) <<  4) | (x & 0x0F0F'0F0F'0F0F'0F0F);

    return x;
}

}  // namespace eggcpt::bit
