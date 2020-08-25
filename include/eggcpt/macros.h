#pragma once

#include <cassert>

#include <eggcpt/env.h>

#if EGGCPT_CC_MSVC
#  define EGGCPT_INLINE    __forceinline
#  define EGGCPT_NO_INLINE __declspec(noinline)
#  define EGGCPT_FUNCTION  __FUNCSIG__
#else
#  define EGGCPT_INLINE    __attribute__((always_inline))
#  define EGGCPT_NO_INLINE __attribute__((noinline))
#  define EGGCPT_FUNCTION  __PRETTY_FUNCTION__
#endif

#define EGGCPT_UNUSED(variable) static_cast<void>(variable)
#define EGGCPT_ASSERT(condition, ...) assert((condition) && (__VA_ARGS__""))

#ifdef NDEBUG
#  define EGGCPT_DEBUG   0
#  define EGGCPT_RELEASE 1
#else
#  define EGGCPT_DEBUG   1
#  define EGGCPT_RELEASE 0
#endif

#if EGGCPT_RELEASE
#  if EGGCPT_CC_MSVC
#    define EGGCPT_UNREACHABLE __assume(0)
#  else
#    define EGGCPT_UNREACHABLE __builtin_unreachable()
#  endif
#else
#  define EGGCPT_UNREACHABLE EGGCPT_ASSERT(false, "Unreachable")
#endif
