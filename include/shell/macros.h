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
