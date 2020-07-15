#pragma once

namespace eggcpt
{

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

constexpr bool kCompilerMsvc       = static_cast<bool>(EGGCPT_COMPILER_MSVC);
constexpr bool kCompilerGcc        = static_cast<bool>(EGGCPT_COMPILER_GCC);
constexpr bool kCompilerClang      = static_cast<bool>(EGGCPT_COMPILER_CLANG);
constexpr bool kCompilerEmscripten = static_cast<bool>(EGGCPT_COMPILER_EMSCRIPTEN);

}  // namespace eggcpt
