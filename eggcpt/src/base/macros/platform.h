#pragma once

namespace eggcpt
{

#ifdef _WIN32
#define EGGCPT_PLATFORM_WINDOWS 1
#else
#define EGGCPT_PLATFORM_WINDOWS 0
#endif

#ifdef __linux__
#define EGGCPT_PLATFORM_LINUX 1
#else
#define EGGCPT_PLATFORM_LINUX 0
#endif

#ifdef __APPLE__
#define EGGCPT_PLATFORM_DARWIN 1
#else
#define EGGCPT_PLATFORM_DARWIN 0
#endif

constexpr bool kPlatformWindows = static_cast<bool>(EGGCPT_PLATFORM_WINDOWS);
constexpr bool kPlatformLinux   = static_cast<bool>(EGGCPT_PLATFORM_LINUX);
constexpr bool kPlatformDarwin  = static_cast<bool>(EGGCPT_PLATFORM_DARWIN);

}  // namespace eggcpt
