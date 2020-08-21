#pragma once

#include <cstdint>

namespace eggcpt
{

using s8   = std::int8_t;
using u8   = std::uint8_t;
using s16  = std::int16_t;
using u16  = std::uint16_t;
using s32  = std::int32_t;
using u32  = std::uint32_t;
using s64  = std::int64_t;
using u64  = std::uint64_t;
using uint = unsigned int;

template<uint Size, uint Unsigned>
struct stdint {};

template<> struct stdint<1, 0> { using type =  s8; };
template<> struct stdint<1, 1> { using type =  u8; };
template<> struct stdint<2, 0> { using type = s16; };
template<> struct stdint<2, 1> { using type = u16; };
template<> struct stdint<4, 0> { using type = s32; };
template<> struct stdint<4, 1> { using type = u32; };
template<> struct stdint<8, 0> { using type = s64; };
template<> struct stdint<8, 1> { using type = u64; };

template<uint Size, uint Unsigned>
using stdint_t = typename stdint<Size, Unsigned>::type;

}  // namespace eggcpt
