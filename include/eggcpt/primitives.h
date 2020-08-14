#pragma once

#include <cstdint>
#include <type_traits>

namespace eggcpt
{

namespace primitives
{

using s8   = std::int8_t;
using u8   = std::uint8_t;
using s16  = std::int16_t;
using u16  = std::uint16_t;
using s32  = std::int32_t;
using u32  = std::uint32_t;
using s64  = std::int64_t;
using u64  = std::uint64_t;
using f32  = float;
using f64  = double;
using uint = unsigned int;

static_assert(sizeof( s8) == 1 && std::is_signed_v<s8>);
static_assert(sizeof( u8) == 1 && std::is_unsigned_v<u8>);
static_assert(sizeof(s16) == 2 && std::is_signed_v<s16>);
static_assert(sizeof(u16) == 2 && std::is_unsigned_v<u16>);
static_assert(sizeof(s32) == 4 && std::is_signed_v<s32>);
static_assert(sizeof(u32) == 4 && std::is_unsigned_v<u32>);
static_assert(sizeof(s64) == 8 && std::is_signed_v<s64>);
static_assert(sizeof(u64) == 8 && std::is_unsigned_v<u64>);
static_assert(sizeof(f32) == 4 && std::is_signed_v<f32>);
static_assert(sizeof(f64) == 8 && std::is_signed_v<f64>);

}  // namespace primitives

using primitives::s8;
using primitives::u8;
using primitives::s16;
using primitives::u16;
using primitives::s32;
using primitives::u32;
using primitives::s64;
using primitives::u64;
using primitives::f32;
using primitives::f64;
using primitives::uint;

}  // namespace eggcpt
