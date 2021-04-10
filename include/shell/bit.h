#pragma once

#include <climits>

#include <shell/int.h>
#include <shell/macros.h>
#include <shell/predef.h>
#include <shell/ranges.h>

#if SHELL_CC_MSVC
#  include <intrin.h>
#elif !SHELL_CC_EMSCRIPTEN 
#  include <x86intrin.h>
#endif

namespace shell::bit
{

template<typename T>
struct bits : std::integral_constant<uint, CHAR_BIT * sizeof(T)> {};

template<typename T>
inline constexpr uint bits_v = bits<T>::value;

template<uint kSize, typename Integral>
constexpr Integral ones()
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(kSize <= bits_v<Integral>);

    if constexpr (kSize == bits_v<Integral>)
        return static_cast<Integral>(~0ULL);
    else
        return static_cast<Integral>((1ULL << kSize) - 1);
}

template<typename Integral>
constexpr Integral ones(uint size)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(size <= bits_v<Integral>);

    if (size == bits_v<Integral>)
        return static_cast<Integral>(~0ULL);
    else
        return static_cast<Integral>((1ULL << size) - 1);
}

template<uint kIndex, uint kSize, typename Integral>
constexpr Integral mask()
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(kIndex + kSize <= bits_v<Integral>);
    static_assert(kIndex < bits_v<Integral>);

    return ones<kSize, Integral>() << kIndex;
}

template<typename Integral>
constexpr Integral mask(uint index, uint size)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(index + size <= bits_v<Integral>);
    SHELL_ASSERT(index < bits_v<Integral>);

    return ones<Integral>(size) << index;
}

template<uint kIndex, uint kSize, typename Integral>
constexpr Integral seq(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(kIndex + kSize <= bits_v<Integral>);

    return (value >> kIndex) & ones<kSize, Integral>();
}

template<typename Integral>
constexpr Integral seq(Integral value, uint index, uint size)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(index + size <= bits_v<Integral>);

    return (value >> index) & ones<Integral>(size);
}

template<uint kIndex, typename Integral>
constexpr Integral byte(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(kIndex < sizeof(Integral));

    return static_cast<unsigned char>(value >> (CHAR_BIT * kIndex));
}

template<typename Integral>
constexpr Integral byte(Integral value, uint index)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(index < sizeof(Integral));

    return static_cast<unsigned char>(value >> (CHAR_BIT * index));
}

template<uint kIndex, typename Integral, typename Byte = unsigned char>
Byte& byteRef(Integral& value)
{
    static_assert(sizeof(Byte) == 1);
    static_assert(std::is_integral_v<Integral>);
    static_assert(kIndex < sizeof(Integral));

    return reinterpret_cast<Byte*>(&value)[kIndex];
}

template<typename Integral, typename Byte = unsigned char>
Byte& byteRef(Integral& value, uint index)
{
    static_assert(sizeof(Byte) == 1);
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(index < sizeof(Integral));

    return reinterpret_cast<Byte*>(&value)[index];
}

template<uint kIndex, typename Integral>
constexpr Integral nibble(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(kIndex < 2 * sizeof(Integral));

    return (value >> ((CHAR_BIT / 2) * kIndex)) & 0xF;
}

template<typename Integral>
constexpr Integral nibble(Integral value, uint index)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(index < 2 * sizeof(Integral));

    return (value >> ((CHAR_BIT / 2) * index)) & 0xF;
}

template<uint kAmount, typename Integral>
constexpr Integral sar(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(kAmount < bits_v<Integral>);

    return static_cast<std::make_signed_t<Integral>>(value) >> kAmount;
}

template<typename Integral>
constexpr Integral sar(Integral value, uint amount)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(amount < bits_v<Integral>);

    return static_cast<std::make_signed_t<Integral>>(value) >> amount;
}

template<uint kAmount, typename Integral>
constexpr Integral shr(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(kAmount < bits_v<Integral>);

    return static_cast<std::make_unsigned_t<Integral>>(value) >> kAmount;
}

template<typename Integral>
constexpr Integral shr(Integral value, uint amount)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(amount < bits_v<Integral>);

    return static_cast<std::make_unsigned_t<Integral>>(value) >> amount;
}

template<uint kSize, typename Integral>
constexpr Integral signEx(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(kSize > 0 && kSize <= bits_v<Integral>);

    if constexpr (kSize == bits_v<Integral>)
    {
        return value;
    }
    else if constexpr (kSize == 8 || kSize == 16 || kSize == 32)
    {
        return static_cast<std::make_signed_t<Integral>>(static_cast<stdint_t<kSize / 8>>(value));
    }
    else
    {
        constexpr uint kShift = bits_v<Integral> - kSize;

        return sar(value << kShift, kShift);
    }
}

template<typename Integral>
constexpr Integral signEx(Integral value, uint size)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(size <= bits_v<Integral>);

    uint shift = bits_v<Integral> - size;

    return sar(value << shift, shift);
}

template<typename Integral>
constexpr Integral msb(Integral value)
{
    static_assert(std::is_integral_v<Integral>);

    return shr(value, bits_v<Integral> - 1);
}

template<typename Integral>
constexpr Integral twos(Integral value)
{
    return ~value + 1;
}

template<typename Integral>
Integral ror(Integral value, uint amount)
{
    static_assert(std::is_integral_v<Integral>);

    #if SHELL_CC_MSVC
    if constexpr (sizeof(Integral) == 1) return _rotr8 (value, amount);
    if constexpr (sizeof(Integral) == 2) return _rotr16(value, amount);
    if constexpr (sizeof(Integral) == 4) return _rotr  (value, amount);
    if constexpr (sizeof(Integral) == 8) return _rotr64(value, amount);
    #elif SHELL_CC_CLANG
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

    #if SHELL_CC_MSVC
    if constexpr (sizeof(Integral) == 1) return _rotl8 (value, amount);
    if constexpr (sizeof(Integral) == 2) return _rotl16(value, amount);
    if constexpr (sizeof(Integral) == 4) return _rotl  (value, amount);
    if constexpr (sizeof(Integral) == 8) return _rotl64(value, amount);
    #elif SHELL_CC_CLANG
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
Integral byteSwap(Integral value)
{
    static_assert(std::is_integral_v<Integral>);

    if constexpr (sizeof(Integral) == 1) return value;
    #if SHELL_CC_MSVC
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
Integral bitSwap(Integral value)
{
    static_assert(std::is_integral_v<Integral>);

    if constexpr (sizeof(Integral) == 1)
    {
        value = shr(value & 0xF0, 4) | (value & 0x0F) << 4;
        value = shr(value & 0xCC, 2) | (value & 0x33) << 2;
        value = shr(value & 0xAA, 1) | (value & 0x55) << 1;
    }
    else
    {
        value = byteSwap(value);

        for (auto& byte : PointerRange(reinterpret_cast<unsigned char*>(&value), sizeof(Integral)))
        {
            byte = bitSwap(byte);
        }
    }
    return value;
}

template<typename Integral>
uint popcnt(Integral value)
{
    static_assert(std::is_integral_v<Integral>);

    #if SHELL_CC_MSVC
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
    SHELL_ASSERT(value != 0);

    #if SHELL_CC_MSVC
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
    SHELL_ASSERT(value != 0);

    #if SHELL_CC_MSVC
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
Integral ceilPowTwo(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(value != 0);

    return 1ULL << (bits_v<Integral> - clz(value - 1));
}

template<typename Integral>
class BitIterator
{
public:
    static_assert(std::is_integral_v<Integral>);

    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = uint;
    using reference         = value_type&;
    using pointer           = value_type*;

    BitIterator(Integral value)
        : _value(value) {}

    uint operator*() const
    {
        return ctz(_value);
    }

    BitIterator& operator++()
    {
        _value &= _value - 1;
        return *this;
    }

    bool operator==(BitIterator other) const
    {
        return _value == other._value;
    }
    
    bool operator!=(BitIterator other) const
    {
        return !(*this == other);
    }

    bool operator==(Sentinel) const
    {
        return _value == 0;
    }

    bool operator!=(Sentinel) const
    {
        return !(*this == Sentinel{});
    }

private:
    Integral _value;
};

template<typename Integral>
SentinelRange<BitIterator<Integral>> iterate(Integral value)
{
    using Iterator = BitIterator<Integral>;

    return { Iterator(value) };
}

}  // namespace shell::bit
