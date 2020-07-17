#pragma once

#include <eggcpt/detail/bit/bit.h>
#include <eggcpt/detail/macro/compiler.h>
#include <eggcpt/detail/macro/macro.h>

#if EGGCPT_COMPILER_MSVC
#include <intrin.h>
#elif !EGGCPT_COMPILER_EMSCRIPTEN 
#include <x86intrin.h>
#endif

namespace eggcpt::bit
{

template<typename T>
T ror(T value, base::uint amount)
{
    static_assert(std::is_integral_v<T>);

    #if EGGCPT_COMPILER_MSVC
    if constexpr (sizeof(T) == 1) return _rotr8 (value, amount);
    if constexpr (sizeof(T) == 2) return _rotr16(value, amount);
    if constexpr (sizeof(T) == 4) return _rotr  (value, amount);
    if constexpr (sizeof(T) == 8) return _rotr64(value, amount);
    #elif EGGCPT_COMPILER_CLANG
    if constexpr (sizeof(T) == 1) return __builtin_rotateright8 (value, amount);
    if constexpr (sizeof(T) == 2) return __builtin_rotateright16(value, amount);
    if constexpr (sizeof(T) == 4) return __builtin_rotateright32(value, amount);
    if constexpr (sizeof(T) == 8) return __builtin_rotateright64(value, amount);
    #else
    constexpr T kMask = bits<T>() - 1;
    amount &= kMask;
    return (value >> amount) | (value << (-amount & kMask));   
    #endif
}

template<typename T>
T rol(T value, base::uint amount)
{
    static_assert(std::is_integral_v<T>);

    #if EGGCPT_COMPILER_MSVC
    if constexpr (sizeof(T) == 1) return _rotl8 (value, amount);
    if constexpr (sizeof(T) == 2) return _rotl16(value, amount);
    if constexpr (sizeof(T) == 4) return _rotl  (value, amount);
    if constexpr (sizeof(T) == 8) return _rotl64(value, amount);
    #elif EGGCPT_COMPILER_CLANG
    if constexpr (sizeof(T) == 1) return __builtin_rotateleft8 (value, amount);
    if constexpr (sizeof(T) == 2) return __builtin_rotateleft16(value, amount);
    if constexpr (sizeof(T) == 4) return __builtin_rotateleft32(value, amount);
    if constexpr (sizeof(T) == 8) return __builtin_rotateleft64(value, amount);
    #else
    constexpr T kMask = bits<T>() - 1;
    amount &= kMask;
    return (value << amount) | (value >> (-amount & kMask));
    #endif
}

template<typename T>
T bswap(T value)
{
    static_assert(std::is_integral_v<T> && sizeof(T) > 1);

    #if EGGCPT_COMPILER_MSVC
    if constexpr (sizeof(T) == 2) return _byteswap_ushort(value);
    if constexpr (sizeof(T) == 4) return _byteswap_ulong (value);
    if constexpr (sizeof(T) == 8) return _byteswap_uint64(value);
    #else
    if constexpr (sizeof(T) == 2) return __builtin_bswap16(value);
    if constexpr (sizeof(T) == 4) return __builtin_bswap32(value);
    if constexpr (sizeof(T) == 8) return __builtin_bswap64(value);
    #endif
}

template<typename T>
base::uint clz(T value)
{
    static_assert(std::is_integral_v<T>);
    EGGCPT_ASSERT(value != 0, "Undefined");

    #if EGGCPT_COMPILER_MSVC
    unsigned long index;
    if constexpr (sizeof(T) <= sizeof(unsigned long))
        _BitScanReverse(&index, value);
    else
        _BitScanReverse64(&index, value);
    return bits<T>() - static_cast<base::uint>(index) - 1;
    #else
    if constexpr (sizeof(T) <= sizeof(unsigned int)
        return __builtin_clz(value);
    else
        return __builtin_clzll(value);
    #endif
}

template<typename T>
base::uint ctz(T value)
{
    static_assert(std::is_integral_v<T>);
    EGGCPT_ASSERT(value != 0, "Undefined");

    #if EGGCPT_COMPILER_MSVC
    unsigned long index;
    if constexpr (sizeof(T) <= sizeof(unsigned long))
        _BitScanForward(&index, value);
    else
        _BitScanForward64(&index, value);
    return static_cast<base::uint>(index);
    #else
    if constexpr (sizeof(T) <= sizeof(unsigned int))
        return __builtin_ctz(value);
    else
        return __builtin_ctzll(value);
    #endif
}

template<typename T>
base::uint popcnt(T value)
{
    static_assert(std::is_integral_v<T>);

    #if EGGCPT_COMPILER_MSVC
    if constexpr (sizeof(T) <= 2) return __popcnt16(value);
    if constexpr (sizeof(T) == 4) return __popcnt  (value);
    if constexpr (sizeof(T) == 8) return __popcnt64(value);
    #else
    if constexpr (sizeof(T) <= sizeof(unsigned int))
        return __builtin_popcount(value);
    else
        return __builtin_popcountll(value);
    #endif
}

}  // namespace eggcpt::bit
