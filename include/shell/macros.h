#pragma once

#include <cassert>

#include <shell/predef.h>

#if SHELL_CC_MSVC
#  define SHELL_INLINE    __forceinline
#  define SHELL_NO_INLINE __declspec(noinline)
#  define SHELL_FUNCTION  __FUNCSIG__
#else
#  define SHELL_INLINE    inline __attribute__((always_inline))
#  define SHELL_NO_INLINE __attribute__((noinline))
#  define SHELL_FUNCTION  __PRETTY_FUNCTION__
#endif

#define SHELL_ARG(...) __VA_ARGS__
#define SHELL_UNUSED(variable) static_cast<void>(variable)
#define SHELL_ASSERT(condition, ...) assert((condition) && #__VA_ARGS__"")

#if SHELL_RELEASE
#  if SHELL_CC_MSVC
#    define SHELL_UNREACHABLE __assume(0)
#  else
#    define SHELL_UNREACHABLE __builtin_unreachable()
#  endif
#else
#  define SHELL_UNREACHABLE SHELL_ASSERT(false, "Unreachable")
#endif

#define SHELL_INDEX_CASE01(label, index, ...)   \
    case label + index:                         \
    {                                           \
        constexpr auto kIndex = index;          \
        constexpr auto kLabel = label + index;  \
        __VA_ARGS__;                            \
        break;                                  \
    }

#define SHELL_INDEX_CASE02(label, index, ...) SHELL_INDEX_CASE01(label, index + 0, __VA_ARGS__) SHELL_INDEX_CASE01(label, index +  1, __VA_ARGS__)
#define SHELL_INDEX_CASE04(label, index, ...) SHELL_INDEX_CASE02(label, index + 0, __VA_ARGS__) SHELL_INDEX_CASE02(label, index +  2, __VA_ARGS__)
#define SHELL_INDEX_CASE08(label, index, ...) SHELL_INDEX_CASE04(label, index + 0, __VA_ARGS__) SHELL_INDEX_CASE04(label, index +  4, __VA_ARGS__)
#define SHELL_INDEX_CASE16(label, index, ...) SHELL_INDEX_CASE08(label, index + 0, __VA_ARGS__) SHELL_INDEX_CASE08(label, index +  8, __VA_ARGS__)
#define SHELL_INDEX_CASE32(label, index, ...) SHELL_INDEX_CASE16(label, index + 0, __VA_ARGS__) SHELL_INDEX_CASE16(label, index + 16, __VA_ARGS__)
#define SHELL_INDEX_CASE64(label, index, ...) SHELL_INDEX_CASE32(label, index + 0, __VA_ARGS__) SHELL_INDEX_CASE32(label, index + 32, __VA_ARGS__)

#define SHELL_CASE01(label, ...) SHELL_INDEX_CASE01(label, 0, __VA_ARGS__)
#define SHELL_CASE02(label, ...) SHELL_INDEX_CASE02(label, 0, __VA_ARGS__)
#define SHELL_CASE04(label, ...) SHELL_INDEX_CASE04(label, 0, __VA_ARGS__)
#define SHELL_CASE08(label, ...) SHELL_INDEX_CASE08(label, 0, __VA_ARGS__)
#define SHELL_CASE16(label, ...) SHELL_INDEX_CASE16(label, 0, __VA_ARGS__)
#define SHELL_CASE32(label, ...) SHELL_INDEX_CASE32(label, 0, __VA_ARGS__)
#define SHELL_CASE64(label, ...) SHELL_INDEX_CASE64(label, 0, __VA_ARGS__)
