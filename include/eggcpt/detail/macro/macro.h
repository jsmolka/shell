#pragma once

#include <cassert>
#include <eggcpt/detail/macro/compiler.h>

#ifdef NDEBUG
#define EGGCPT_DEBUG   0
#define EGGCPT_RELEASE 1
#else
#define EGGCPT_DEBUG   1
#define EGGCPT_RELEASE 0
#endif

#define EGGCPT_ASSERT(condition, message) assert((condition) && message)
#define EGGCPT_UNUSED(variable) static_cast<void>(variable)

#if EGGCPT_COMPILER_MSVC
#define EGGCPT_INLINE __forceinline
#else
#define EGGCPT_INLINE __attribute__((always_inline))
#endif

#if EGGCPT_COMPILER_MSVC
#define EGGCPT_NO_INLINE __declspec(noinline)
#else
#define EGGCPT_NO_INLINE __attribute__((noinline))
#endif

#if EGGCPT_RELEASE
#  if EGGCPT_COMPILER_MSVC
#  define EGGCPT_UNREACHABLE __assume(0)
#  else
#  define EGGCPT_UNREACHABLE __builtin_unreachable()
#  endif
#else
#  define EGGCPT_UNREACHABLE EGGCPT_ASSERT(false, "Unreachable")
#endif
