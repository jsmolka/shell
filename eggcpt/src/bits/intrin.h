#pragma once

#include <climits>

#include "base.h"
#include "traits.h"

#if EGGCPT_COMPILER_MSVC
#include <intrin.h>
#elif !EGGCPT_COMPILER_EMSCRIPTEN 
#include <x86intrin.h>
#endif

namespace eggcpt::bits
{

template<typename T>
T ror(T value, uint amount)
{
    static_assert(traits::is_integer_v<T>);
    static_assert(traits::is_size_max_v<T, 8>);

    if constexpr (kCompilerMsvc)
    {
             if constexpr (sizeof(T) == 1) return _rotr8 (value, amount);
        else if constexpr (sizeof(T) == 2) return _rotr16(value, amount);
        else if constexpr (sizeof(T) == 4) return _rotr  (value, amount);
        else if constexpr (sizeof(T) == 8) return _rotr64(value, amount);
        else static_assert(false);
    }
    else if constexpr (kCompilerClang)
    {
             if constexpr (sizeof(T) == 1) return __builtin_rotateright8 (value, amount);
        else if constexpr (sizeof(T) == 2) return __builtin_rotateright16(value, amount);
        else if constexpr (sizeof(T) == 4) return __builtin_rotateright32(value, amount);
        else if constexpr (sizeof(T) == 8) return __builtin_rotateright64(value, amount);
        else static_assert(false);
    }
    else
    {
        constexpr T mask = CHAR_BIT * sizeof(T) - 1;
        amount &= mask;
        return (value >> amount) | (value << (-amount & mask));   
    }
}

template<typename T>
T rol(T value, uint amount)
{
    static_assert(traits::is_integer_v<T>);
    static_assert(traits::is_size_max_v<T, 8>);

    if constexpr (kCompilerMsvc)
    {
             if constexpr (sizeof(T) == 1) return _rotl8 (value, amount);
        else if constexpr (sizeof(T) == 2) return _rotl16(value, amount);
        else if constexpr (sizeof(T) == 4) return _rotl  (value, amount);
        else if constexpr (sizeof(T) == 8) return _rotl64(value, amount);
        else static_assert(false);
    }
    else if constexpr (kCompilerClang)
    {
             if constexpr (sizeof(T) == 1) return __builtin_rotateleft8 (value, amount);
        else if constexpr (sizeof(T) == 2) return __builtin_rotateleft16(value, amount);
        else if constexpr (sizeof(T) == 4) return __builtin_rotateleft32(value, amount);
        else if constexpr (sizeof(T) == 8) return __builtin_rotateleft64(value, amount);
        else static_assert(false);
    }
    else
    {
        constexpr T mask = CHAR_BIT * sizeof(T) - 1;
        amount &= mask;
        return (value << amount) | (value >> (-amount & mask));
    }
}

template<typename T>
uint clz(T value)
{
    static_assert(traits::is_integer_v<T>);
    static_assert(traits::is_size_max_v<T, 8>);

    if constexpr (kCompilerMsvc)
    {
        unsigned long index;
        if constexpr (sizeof(T) <= 4)
            _BitScanReverse(&index, value);
        else
            _BitScanReverse64(&index, value);
        return (CHAR_BIT * sizeof(T) - 1) - static_cast<uint>(index);
    }
    else
    {
        if constexpr (sizeof(T) <= 4)
            return __builtin_clz(value);
        else
            return __builtin_clzll(value);
    }
}

template<typename T>
uint ctz(T value)
{
    static_assert(traits::is_integer_v<T>);
    static_assert(traits::is_size_max_v<T, 8>);

    if constexpr (kCompilerMsvc)
    {
        unsigned long index;
        if constexpr (sizeof(T) <= 4)
            _BitScanForward(&index, value);
        else
            _BitScanForward64(&index, value);
        return static_cast<uint>(index);
    }
    else
    {
        if constexpr (sizeof(T) <= 4)
            return __builtin_ctz(value);
        else
            return __builtin_ctzll(value);
    }
}

template<typename T>
uint popcnt(T value)
{
    static_assert(traits::is_integer_v<T>);
    static_assert(traits::is_size_max_v<T, 8>);

    if constexpr (kCompilerMsvc)
    {
             if constexpr (sizeof(T) <= 2) return __popcnt16(value);
        else if constexpr (sizeof(T) == 4) return __popcnt  (value);
        else if constexpr (sizeof(T) == 8) return __popcnt64(value);
        else static_assert(false);
    }
    else
    {
        if constexpr (sizeof(T) <= 4)
            return __builtin_popcount(value);
        else
            return __builtin_popcountll(value);
    }
}

template<typename T>
T bswap(T value)
{
    static_assert(traits::is_integer_v<T>);
    static_assert(traits::is_size_min_v<T, 2>);
    static_assert(traits::is_size_max_v<T, 8>);

    if constexpr (kCompilerMsvc)
    {
             if constexpr (sizeof(T) == 2) return _byteswap_ushort(value);
        else if constexpr (sizeof(T) == 4) return _byteswap_ulong (value);
        else if constexpr (sizeof(T) == 8) return _byteswap_uint64(value);
        else static_assert(false);
    }
    else
    {
             if constexpr (sizeof(T) == 2) return __builtin_bswap16(value);
        else if constexpr (sizeof(T) == 4) return __builtin_bswap32(value);
        else if constexpr (sizeof(T) == 8) return __builtin_bswap64(value);
        else static_assert(false);
    }
}

}  // namespace eggcpt::bits
