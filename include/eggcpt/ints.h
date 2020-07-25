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

static_assert(sizeof( s8) == 1);
static_assert(sizeof( u8) == 1);
static_assert(sizeof(s16) == 2);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(s32) == 4);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(s64) == 8);
static_assert(sizeof(u64) == 8);

}  // namespace eggcpt
