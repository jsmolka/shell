#pragma once

#include <cassert>

#ifdef _MSC_VER
#  define EGGCPT_CC_MSVC 1
#else
#  define EGGCPT_CC_MSVC 0
#endif

#ifdef __GNUC__
#  define EGGCPT_CC_GCC 1
#else
#  define EGGCPT_CC_GCC 0
#endif

#ifdef __MINGW32__
#  define EGGCPT_CC_MINGW 1
#else
#  define EGGCPT_CC_MINGW 0
#endif

#ifdef __clang__
#  define EGGCPT_CC_CLANG 1
#else
#  define EGGCPT_CC_CLANG 0
#endif

#ifdef __EMSCRIPTEN__
#  define EGGCPT_CC_EMSCRIPTEN 1
#else
#  define EGGCPT_CC_EMSCRIPTEN 0
#endif

#if !(EGGCPT_CC_MSVC || EGGCPT_CC_GCC || EGGCPT_CC_MINGW || EGGCPT_CC_CLANG || EGGCPT_CC_EMSCRIPTEN)
#  error Unsupported compiler
#endif

#ifdef _WIN32
#define EGGCPT_OS_WINDOWS 1
#else
#define EGGCPT_OS_WINDOWS 0
#endif

#ifdef __linux__
#define EGGCPT_OS_LINUX 1
#else
#define EGGCPT_OS_LINUX 0
#endif

#ifdef __APPLE__
#define EGGCPT_OS_DARWIN 1
#else
#define EGGCPT_OS_DARWIN 0
#endif

#if !(EGGCPT_OS_WINDOWS || EGGCPT_OS_LINUX || EGGCPT_OS_DARWIN)
#error Unsupported platform
#endif

#ifdef NDEBUG
#  define EGGCPT_DEBUG   0
#  define EGGCPT_RELEASE 1
#else
#  define EGGCPT_DEBUG   1
#  define EGGCPT_RELEASE 0
#endif

#define EGGCPT_ASSERT(condition, message) assert((condition) && message)
#define EGGCPT_UNUSED(variable) static_cast<void>(variable)

#if EGGCPT_CC_MSVC
#  define EGGCPT_INLINE    __forceinline
#  define EGGCPT_NO_INLINE __declspec(noinline)
#  define EGGCPT_FUNCTION  __FUNCSIG__
#else
#  define EGGCPT_INLINE    __attribute__((always_inline))
#  define EGGCPT_NO_INLINE __attribute__((noinline))
#  define EGGCPT_FUNCTION  __PRETTY_FUNCTION__
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

