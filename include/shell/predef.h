#pragma once

#ifdef NDEBUG
#  define SHELL_DEBUG   0
#  define SHELL_RELEASE 1
#else
#  define SHELL_DEBUG   1
#  define SHELL_RELEASE 0
#endif

#ifdef _MSC_VER
#  define SHELL_CC_MSVC 1
#else
#  define SHELL_CC_MSVC 0
#endif

#ifdef __clang__
#  define SHELL_CC_CLANG 1
#else
#  define SHELL_CC_CLANG 0
#endif

#if defined(__GNUC__) && !SHELL_CC_CLANG
#  define SHELL_CC_GCC 1
#else
#  define SHELL_CC_GCC 0
#endif

#ifdef __EMSCRIPTEN__
#  define SHELL_CC_EMSCRIPTEN 1
#else
#  define SHELL_CC_EMSCRIPTEN 0
#endif

#ifdef __MINGW32__
#  define SHELL_CC_MINGW 1
#else
#  define SHELL_CC_MINGW 0
#endif

#ifdef _WIN32
#  define SHELL_OS_WINDOWS 1
#else
#  define SHELL_OS_WINDOWS 0
#endif

#ifdef __APPLE__
#  define SHELL_OS_MACOS 1
#else
#  define SHELL_OS_MACOS 0
#endif

#ifdef __linux__
#  define SHELL_OS_LINUX 1
#else
#  define SHELL_OS_LINUX 0
#endif

#ifdef __unix__
#  define SHELL_OS_UNIX 1
#else
#  define SHELL_OS_UNIX 0
#endif

#ifdef __FreeBSD_version
#  define SHELL_OS_BSD_FREE 1
#else
#  define SHELL_OS_BSD_FREE 0
#endif

#ifdef __NetBSD__
#  define SHELL_OS_BSD_NET 1
#else
#  define SHELL_OS_BSD_NET 0
#endif

#ifdef __DragonFly__
#  define SHELL_OS_BSD_DRAGONFLY 1
#else
#  define SHELL_OS_BSD_DRAGONFLY 0
#endif

#if defined(_M_X86) || defined(__i386__)
#  define SHELL_ARCH_X86 1
#else
#  define SHELL_ARCH_X86 0
#endif
