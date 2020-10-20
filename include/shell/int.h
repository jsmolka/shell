#pragma once

#include <cstdint>

namespace shell
{

namespace integer
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

template<uint Size>
struct stdint {};

template<> struct stdint<1> { using type =  s8; };
template<> struct stdint<2> { using type = s16; };
template<> struct stdint<4> { using type = s32; };
template<> struct stdint<8> { using type = s64; };

template<uint Size>
using stdint_t = typename stdint<Size>::type;

template<uint Size>
struct stduint {};

template<> struct stduint<1> { using type =  u8; };
template<> struct stduint<2> { using type = u16; };
template<> struct stduint<4> { using type = u32; };
template<> struct stduint<8> { using type = u64; };

template<uint Size>
using stduint_t = typename stduint<Size>::type;

}  // namespace integer

using integer::s8;
using integer::u8;
using integer::s16;
using integer::u16;
using integer::s32;
using integer::u32;
using integer::s64;
using integer::u64;
using integer::uint;
using integer::stdint;
using integer::stdint_t;
using integer::stduint;
using integer::stduint_t;

}  // namespace shell
