#pragma once

#include <climits>

#include <eggcpt/int.h>
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
constexpr typename bits<T>::value_type bits_v = bits<T>::value;

template<uint Size, typename Integral>
constexpr Integral ones()
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Size <= bits_v<Integral>);

    if constexpr (Size == bits_v<Integral>)
        return static_cast<Integral>(~0ull);
    else
        return static_cast<Integral>(~(~0ull << Size));
}

template<uint Index, uint Size, typename Integral>
constexpr Integral mask()
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Index + Size <= bits_v<Integral>);
    static_assert(Index < bits_v<Integral>);

    return ones<Size, Integral>() << Index;
}

template<uint Index, uint Size, typename Integral>
constexpr Integral seq(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Index + Size <= bits_v<Integral>);

    return (value >> Index) & ones<Size, Integral>();
}

template<typename Integral>
constexpr Integral sar(Integral value, uint amount)
{
    static_assert(std::is_integral_v<Integral>);

    return static_cast<std::make_signed_t<Integral>>(value) >> amount;
}

template<typename Integral>
constexpr Integral shr(Integral value, uint amount)
{
    static_assert(std::is_integral_v<Integral>);

    return static_cast<std::make_unsigned_t<Integral>>(value) >> amount;
}

template<typename Integral>
constexpr Integral msb(Integral value)
{
    static_assert(std::is_integral_v<Integral>);

    return shr(value, bits_v<Integral> - 1);
}

template<uint Size, typename Integral>
constexpr Integral signEx(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Size <= bits_v<Integral>);

    constexpr Integral kMask = 1ull << (Size - 1);

    return (value ^ kMask) - kMask;
}

template<typename Integral>
Integral ror(Integral value, uint amount)
{
    static_assert(std::is_integral_v<Integral>);

    #if EGGCPT_CC_MSVC
    if constexpr (sizeof(Integral) == 1) return _rotr8 (value, amount);
    if constexpr (sizeof(Integral) == 2) return _rotr16(value, amount);
    if constexpr (sizeof(Integral) == 4) return _rotr  (value, amount);
    if constexpr (sizeof(Integral) == 8) return _rotr64(value, amount);
    #elif EGGCPT_CC_CLANG
    if constexpr (sizeof(Integral) == 1) return __builtin_rotateright8 (value, amount);
    if constexpr (sizeof(Integral) == 2) return __builtin_rotateright16(value, amount);
    if constexpr (sizeof(Integral) == 4) return __builtin_rotateright32(value, amount);
    if constexpr (sizeof(Integral) == 8) return __builtin_rotateright64(value, amount);
    #else
    constexpr Integral kMask = bits_v<Integral> - 1;

    amount &= kMask;
    return shr(value, amount) | (value << (-amount & kMask));  
    #endif
}

template<typename Integral>
Integral rol(Integral value, uint amount)
{
    static_assert(std::is_integral_v<Integral>);

    #if EGGCPT_CC_MSVC
    if constexpr (sizeof(Integral) == 1) return _rotl8 (value, amount);
    if constexpr (sizeof(Integral) == 2) return _rotl16(value, amount);
    if constexpr (sizeof(Integral) == 4) return _rotl  (value, amount);
    if constexpr (sizeof(Integral) == 8) return _rotl64(value, amount);
    #elif EGGCPT_CC_CLANG
    if constexpr (sizeof(Integral) == 1) return __builtin_rotateleft8 (value, amount);
    if constexpr (sizeof(Integral) == 2) return __builtin_rotateleft16(value, amount);
    if constexpr (sizeof(Integral) == 4) return __builtin_rotateleft32(value, amount);
    if constexpr (sizeof(Integral) == 8) return __builtin_rotateleft64(value, amount);
    #else
    constexpr Integral kMask = bits_v<Integral> - 1;

    amount &= kMask;
    return (value << amount) | shr(value, -amount & kMask);
    #endif
}

template<typename Integral>
Integral bswap(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(sizeof(Integral) > 1);

    #if EGGCPT_CC_MSVC
    if constexpr (sizeof(Integral) == 2) return _byteswap_ushort(value);
    if constexpr (sizeof(Integral) == 4) return _byteswap_ulong (value);
    if constexpr (sizeof(Integral) == 8) return _byteswap_uint64(value);
    #else
    if constexpr (sizeof(Integral) == 2) return __builtin_bswap16(value);
    if constexpr (sizeof(Integral) == 4) return __builtin_bswap32(value);
    if constexpr (sizeof(Integral) == 8) return __builtin_bswap64(value);
    #endif
}

template<typename Integral>
uint popcnt(Integral value)
{
    static_assert(std::is_integral_v<Integral>);

    #if EGGCPT_CC_MSVC
    if constexpr (sizeof(Integral) <= 2) return __popcnt16(value);
    if constexpr (sizeof(Integral) == 4) return __popcnt  (value);
    if constexpr (sizeof(Integral) == 8) return __popcnt64(value);
    #else
    if constexpr (sizeof(Integral) <= 4) return __builtin_popcount  (value);
    if constexpr (sizeof(Integral) == 8) return __builtin_popcountll(value);
    #endif
}

template<typename Integral>
uint clz(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    EGGCPT_ASSERT(value != 0, "Undefined");

    #if EGGCPT_CC_MSVC
    unsigned long index;
    if constexpr (sizeof(Integral) <= 4) _BitScanReverse  (&index, value);
    if constexpr (sizeof(Integral) == 8) _BitScanReverse64(&index, value);
    return bits_v<Integral> - static_cast<uint>(index) - 1;
    #else
    if constexpr (sizeof(Integral) <= 4) return __builtin_clz  (value);
    if constexpr (sizeof(Integral) == 8) return __builtin_clzll(value);
    #endif
}

template<typename Integral>
uint ctz(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    EGGCPT_ASSERT(value != 0, "Undefined");

    #if EGGCPT_CC_MSVC
    unsigned long index;
    if constexpr (sizeof(Integral) <= 4) _BitScanForward  (&index, value);
    if constexpr (sizeof(Integral) == 8) _BitScanForward64(&index, value);
    return static_cast<uint>(index);
    #else
    if constexpr (sizeof(Integral) <= 4) return __builtin_ctz  (value);
    if constexpr (sizeof(Integral) == 8) return __builtin_ctzll(value);
    #endif
}

template<typename Integral>
class BitIterator
{
    static_assert(std::is_integral_v<Integral>);

public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = uint;
    using reference         = uint&;
    using pointer           = uint*;

    explicit BitIterator(Integral value)
        : value(value) {}

    uint operator*() const
    {
        return ctz(value);
    }

    BitIterator& operator++()
    {
        value &= value - 1;
        return *this;
    }

    bool operator==(BitIterator other) const { return value == other.value; }
    bool operator!=(BitIterator other) const { return value != other.value; }

private:
    Integral value;
};

template<typename Integral>
IteratorRange<BitIterator<Integral>> iterate(Integral value)
{
    static_assert(std::is_integral_v<Integral>);

    return makeIteratorRange(
        BitIterator<Integral>(value),
        BitIterator<Integral>(0));
}

}  // namespace eggcpt::bit
