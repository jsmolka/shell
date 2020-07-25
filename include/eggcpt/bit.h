#pragma once

#include <climits>
#include <iterator>
#include <type_traits>

#include <eggcpt/ints.h>
#include <eggcpt/iterator.h>
#include <eggcpt/macros.h>

#if EGGCPT_CC_MSVC
#include <intrin.h>
#elif !EGGCPT_CC_EMSCRIPTEN 
#include <x86intrin.h>
#endif

namespace eggcpt::bit
{

template<typename T>
struct bits : std::integral_constant<uint, CHAR_BIT * sizeof(T)> {};

template<typename T>
constexpr auto bits_v = bits<T>::value;

template<uint Size> struct storage_type {};
template<> struct storage_type<sizeof( u8)> { using type = u8;  };
template<> struct storage_type<sizeof(u16)> { using type = u16; };
template<> struct storage_type<sizeof(u32)> { using type = u32; };
template<> struct storage_type<sizeof(u64)> { using type = u64; };

template<uint Size>
using storage_type_t = typename storage_type<Size>::type;

template<uint Index, uint Size, typename T>
constexpr T subset(T value)
{
    static_assert(std::is_integral_v<T>);
    static_assert(Index + Size <= bits_v<T>);

    constexpr T kMask = ~static_cast<T>(0) >> (bits_v<T> - Size);

    return (value >> Index) & kMask;
}

template<uint Size, typename T>
constexpr T sign_ex(T value)
{
    static_assert(std::is_integral_v<T>);
    static_assert(Size > 0 && Size <= bits_v<T>);

    constexpr T kAmount = bits_v<T> - Size;

    return static_cast<std::make_signed_t<T>>(value << kAmount) >> kAmount;
}

template<typename T>
constexpr T sar(T value, uint amount)
{
    static_assert(std::is_integral_v<T>);

    return static_cast<std::make_signed_t<T>>(value) >> amount;
}

template<typename T>
constexpr T shr(T value, uint amount)
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

template<typename T>
T ror(T value, uint amount)
{
    static_assert(std::is_integral_v<T>);

    #if EGGCPT_CC_MSVC
    if constexpr (sizeof(T) == 1) return _rotr8 (value, amount);
    if constexpr (sizeof(T) == 2) return _rotr16(value, amount);
    if constexpr (sizeof(T) == 4) return _rotr  (value, amount);
    if constexpr (sizeof(T) == 8) return _rotr64(value, amount);
    #elif EGGCPT_CC_CLANG
    if constexpr (sizeof(T) == 1) return __builtin_rotateright8 (value, amount);
    if constexpr (sizeof(T) == 2) return __builtin_rotateright16(value, amount);
    if constexpr (sizeof(T) == 4) return __builtin_rotateright32(value, amount);
    if constexpr (sizeof(T) == 8) return __builtin_rotateright64(value, amount);
    #else
    constexpr T kMask = bits_v<T> - 1;
    amount &= kMask;
    return (value >> amount) | (value << (-amount & kMask));   
    #endif
}

template<typename T>
T rol(T value, uint amount)
{
    static_assert(std::is_integral_v<T>);

    #if EGGCPT_CC_MSVC
    if constexpr (sizeof(T) == 1) return _rotl8 (value, amount);
    if constexpr (sizeof(T) == 2) return _rotl16(value, amount);
    if constexpr (sizeof(T) == 4) return _rotl  (value, amount);
    if constexpr (sizeof(T) == 8) return _rotl64(value, amount);
    #elif EGGCPT_CC_CLANG
    if constexpr (sizeof(T) == 1) return __builtin_rotateleft8 (value, amount);
    if constexpr (sizeof(T) == 2) return __builtin_rotateleft16(value, amount);
    if constexpr (sizeof(T) == 4) return __builtin_rotateleft32(value, amount);
    if constexpr (sizeof(T) == 8) return __builtin_rotateleft64(value, amount);
    #else
    constexpr T kMask = bits_v<T> - 1;
    amount &= kMask;
    return (value << amount) | (value >> (-amount & kMask));
    #endif
}

template<typename T>
T bswap(T value)
{
    static_assert(std::is_integral_v<T>);
    static_assert(sizeof(T) >= 2);

    #if EGGCPT_CC_MSVC
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
uint clz(T value)
{
    static_assert(std::is_integral_v<T>);
    EGGCPT_ASSERT(value != 0, "Undefined");

    #if EGGCPT_CC_MSVC
    unsigned long index;
    if constexpr (sizeof(T) <= sizeof(unsigned long))
        _BitScanReverse(&index, value);
    else
        _BitScanReverse64(&index, value);
    return bits_v<T> - static_cast<unsigned>(index) - 1;
    #else
    if constexpr (sizeof(T) <= sizeof(unsigned int))
        return __builtin_clz(value);
    else
        return __builtin_clzll(value);
    #endif
}

template<typename T>
uint ctz(T value)
{
    static_assert(std::is_integral_v<T>);
    EGGCPT_ASSERT(value != 0, "Undefined");

    #if EGGCPT_CC_MSVC
    unsigned long index;
    if constexpr (sizeof(T) <= sizeof(unsigned long))
        _BitScanForward(&index, value);
    else
        _BitScanForward64(&index, value);
    return static_cast<unsigned>(index);
    #else
    if constexpr (sizeof(T) <= sizeof(unsigned int))
        return __builtin_ctz(value);
    else
        return __builtin_ctzll(value);
    #endif
}

template<typename T>
uint popcnt(T value)
{
    static_assert(std::is_integral_v<T>);

    #if EGGCPT_CC_MSVC
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

template<typename T>
class bit_iterator
{
    static_assert(std::is_integral_v<T>);

public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using value_type = T;

    bit_iterator(T value)
        : value(value) {}

    uint operator*() const
    {
        return ctz(value);
    }

    bit_iterator& operator++()
    {
        value &= value - 1;
        return *this;
    }

    bool operator==(bit_iterator other) const { return value == other.value; }
    bool operator!=(bit_iterator other) const { return value != other.value; }

private:
    T value;
};

template<typename T>
auto iterate(T value)
{
    static_assert(std::is_integral_v<T>);

    return make_iterator_range<bit_iterator<T>>(value, 0);
}

}  // namespace eggcpt::bit
