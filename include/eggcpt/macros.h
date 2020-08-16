#pragma once

#include <cassert>

#define EGGCPT_OS_WINDOWS _WIN32
#define EGGCPT_OS_LINUX   __linux__
#define EGGCPT_OS_DARWIN  __APPLE__

#define EGGCPT_CC_MSVC       _MSC_VER
#define EGGCPT_CC_GCC        __GNUC__
#define EGGCPT_CC_MINGW      __MINGW32__
#define EGGCPT_CC_CLANG      __clang__
#define EGGCPT_CC_EMSCRIPTEN __EMSCRIPTEN__

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
