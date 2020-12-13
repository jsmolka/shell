#pragma once

#include <climits>

#include <shell/iterator.h>
#include <shell/macros.h>
#include <shell/predef.h>

#if SHELL_CC_MSVC
#  include <intrin.h>
#elif !SHELL_CC_EMSCRIPTEN 
#  include <x86intrin.h>
#endif

namespace shell::bit
{

template<typename T>
struct bits : std::integral_constant<std::size_t, CHAR_BIT * sizeof(T)> {};

template<typename T>
inline constexpr std::size_t bits_v = bits<T>::value;

template<std::size_t Size, typename Integral>
constexpr Integral ones()
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Size <= bits_v<Integral>);

    if constexpr (Size == bits_v<Integral>)
        return static_cast<Integral>(~0ULL);
    else
        return static_cast<Integral>((1ULL << Size) - 1);
}

template<typename Integral>
constexpr Integral ones(std::size_t size)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(size <= bits_v<Integral>);

    if (size == bits_v<Integral>)
        return static_cast<Integral>(~0ULL);
    else
        return static_cast<Integral>((1ULL << size) - 1);
}

template<std::size_t Index, std::size_t Size, typename Integral>
constexpr Integral mask()
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Index + Size <= bits_v<Integral>);
    static_assert(Index < bits_v<Integral>);

    return ones<Size, Integral>() << Index;
}

template<typename Integral>
constexpr Integral mask(std::size_t index, std::size_t size)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(index + size <= bits_v<Integral>);
    SHELL_ASSERT(index < bits_v<Integral>);

    return ones<Integral>(size) << index;
}

template<std::size_t Index, std::size_t Size, typename Integral>
constexpr Integral seq(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Index + Size <= bits_v<Integral>);

    return (value >> Index) & ones<Size, Integral>();
}

template<typename Integral>
constexpr Integral seq(Integral value, std::size_t index, std::size_t size)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(index + size <= bits_v<Integral>);

    return (value >> index) & ones<Integral>(size);
}

template<std::size_t Index, typename Integral>
constexpr Integral byte(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Index < sizeof(Integral));

    return static_cast<unsigned char>(value >> (CHAR_BIT * Index));
}

template<typename Integral>
constexpr Integral byte(Integral value, std::size_t index)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(index < sizeof(Integral));

    return static_cast<unsigned char>(value >> (CHAR_BIT * index));
}

template<std::size_t Index, typename Integral>
constexpr Integral nibble(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Index < 2 * sizeof(Integral));

    return (value >> ((CHAR_BIT / 2) * Index)) & 0xF;
}

template<typename Integral>
constexpr Integral nibble(Integral value, std::size_t index)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(index < 2 * sizeof(Integral));

    return (value >> ((CHAR_BIT / 2) * index)) & 0xF;
}

template<std::size_t Size, typename Integral>
constexpr Integral signEx(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Size <= bits_v<Integral>);

    constexpr Integral kMask = 1ULL << (Size - 1);

    return (value ^ kMask) - kMask;
}

template<typename Integral>
constexpr Integral signEx(Integral value, std::size_t size)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(size <= bits_v<Integral>);

    Integral mask = 1ULL << (size - 1);

    return (value ^ mask) - mask;
}

template<std::size_t Amount, typename Integral>
constexpr Integral sar(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Amount < bits_v<Integral>);

    return static_cast<std::make_signed_t<Integral>>(value) >> Amount;
}

template<typename Integral>
constexpr Integral sar(Integral value, std::size_t amount)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(amount < bits_v<Integral>);

    return static_cast<std::make_signed_t<Integral>>(value) >> amount;
}

template<std::size_t Amount, typename Integral>
constexpr Integral shr(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Amount < bits_v<Integral>);

    return static_cast<std::make_unsigned_t<Integral>>(value) >> Amount;
}

template<typename Integral>
constexpr Integral shr(Integral value, std::size_t amount)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(amount < bits_v<Integral>);

    return static_cast<std::make_unsigned_t<Integral>>(value) >> amount;
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
Integral ror(Integral value, std::size_t amount)
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
Integral rol(Integral value, std::size_t amount)
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
        value = (value & 0xF0) >> 4 | (value & 0x0F) << 4;
        value = (value & 0xCC) >> 2 | (value & 0x33) << 2;
        value = (value & 0xAA) >> 1 | (value & 0x55) << 1;
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
std::size_t popcnt(Integral value)
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
std::size_t clz(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(value != 0, "Undefined");

    #if SHELL_CC_MSVC
    unsigned long index;
    if constexpr (sizeof(Integral) <= 4) _BitScanReverse  (&index, value);
    if constexpr (sizeof(Integral) == 8) _BitScanReverse64(&index, value);
    return bits_v<Integral> - static_cast<std::size_t>(index) - 1;
    #else
    if constexpr (sizeof(Integral) <= 4) return __builtin_clz  (value);
    if constexpr (sizeof(Integral) == 8) return __builtin_clzll(value);
    #endif
}

template<typename Integral>
std::size_t ctz(Integral value)
{
    static_assert(std::is_integral_v<Integral>);
    SHELL_ASSERT(value != 0, "Undefined");

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
    SHELL_ASSERT(value != 0, "Undefined");

    return 1ULL << (bits_v<Integral> - clz(value - 1));
}

template<typename Integral>
class BitIterator
{
public:
    static_assert(std::is_integral_v<Integral>);

    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = std::size_t;
    using reference         = std::size_t&;
    using pointer           = std::size_t*;

    explicit BitIterator(Integral value)
        : value(value) {}

    std::size_t operator*() const
    {
        return ctz(value);
    }

    BitIterator& operator++()
    {
        value &= value - 1;
        return *this;
    }

    bool operator==(BitIterator other) const
    {
        return value == other.value;
    }
    
    bool operator!=(BitIterator other) const
    {
        return value != other.value;
    }

private:
    Integral value;
};

template<typename Integral>
IteratorRange<BitIterator<Integral>> iterate(Integral value)
{
    static_assert(std::is_integral_v<Integral>);

    return IteratorRange(
        BitIterator<Integral>(value),
        BitIterator<Integral>(0));
}

}  // namespace shell::bit
