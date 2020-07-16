#pragma once

#ifdef _MSC_VER
#define EGGCPT_COMPILER_MSVC 1
#else
#define EGGCPT_COMPILER_MSVC 0
#endif

#ifdef __GNUC__
#define EGGCPT_COMPILER_GCC 1
#else
#define EGGCPT_COMPILER_GCC 0
#endif

#ifdef __clang__
#define EGGCPT_COMPILER_CLANG 1
#else
#define EGGCPT_COMPILER_CLANG 0
#endif

#ifdef __EMSCRIPTEN__
#define EGGCPT_COMPILER_EMSCRIPTEN 1
#else
#define EGGCPT_COMPILER_EMSCRIPTEN 0
#endif

#if !(EGGCPT_COMPILER_MSVC || EGGCPT_COMPILER_GCC || EGGCPT_COMPILER_CLANG || EGGCPT_COMPILER_EMSCRIPTEN)
#error Unsupported compiler
#endif
