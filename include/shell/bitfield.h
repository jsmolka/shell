#pragma once

#include <shell/bit.h>
#include <shell/macros.h>

namespace shell
{

template<uint Index, uint Size, typename Integral>
class BitField
{
    static_assert(std::is_integral_v<Integral>);
    static_assert(Index + Size <= bit::bits_v<Integral>);
    static_assert(Index < bit::bits_v<Integral>);

public:
    BitField& operator=(Integral value)
    {
        data = (data & ~(kMask << Index)) | ((value & kMask) << Index);
        return *this;
    }

    BitField& operator|=(Integral value)
    {
        data |= (value & kMask) << Index;
        return *this;
    }

    BitField& operator&=(Integral value)
    {
        data &= (value << Index) | ~(kMask << Index);
        return *this;
    }

    BitField& operator^=(Integral value)
    {
        data ^= (value & kMask) << Index;
        return *this;
    }

    operator Integral() const
    {
        return (data >> Index) & kMask;
    }

    explicit operator bool() const
    {
        return data & (kMask << Index);
    }

    void setUnchecked(Integral value)
    {
        SHELL_ASSERT((value & ~kMask) == 0, "Bad unchecked set");

        data = (data & ~(kMask << Index)) | (value << Index);
    }

    void flip()
    {
        data ^= kMask << Index;
    }

private:
    static constexpr Integral kMask = bit::ones<Size, Integral>();

    Integral data;
};

}  // namespace shell
