#pragma once

#include <cstddef>
#include <cstdint>

namespace shell
{

using uint = unsigned int;

using s8  = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

template<std::size_t Size>
struct stdint {};

template<> struct stdint<1> { using type =  s8; };
template<> struct stdint<2> { using type = s16; };
template<> struct stdint<4> { using type = s32; };
template<> struct stdint<8> { using type = s64; };

template<std::size_t Size>
using stdint_t = typename stdint<Size>::type;

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

template<std::size_t Size>
struct stduint {};

template<> struct stduint<1> { using type =  u8; };
template<> struct stduint<2> { using type = u16; };
template<> struct stduint<4> { using type = u32; };
template<> struct stduint<8> { using type = u64; };

template<std::size_t Size>
using stduint_t = typename stduint<Size>::type;

}  // namespace shell
