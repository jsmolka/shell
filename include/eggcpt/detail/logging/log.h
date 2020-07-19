#pragma once

#include <utility>
#include <eggcpt/detail/logging/logger.h>
#include <eggcpt/detail/macro/compiler.h>

namespace eggcpt::logging
{
    template<typename T, typename... Args>
    void debug(const char* func, int line, const T& format, Args&&... args)
    {
        if constexpr (kLevelDebug >= kLevel)
            logger::instance().log(func, line, kLevelDebug, format, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void info(const char* func, int line, const T& format, Args&&... args)
    {
        if constexpr (kLevelInfo >= kLevel)
            logger::instance().log(func, line, kLevelInfo, format, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void warn(const char* func, int line, const T& format, Args&&... args)
    {
        if constexpr (kLevelWarn >= kLevel)
            logger::instance().log(func, line, kLevelWarn, format, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void error(const char* func, int line, const T& format, Args&&... args)
    {
        if constexpr (kLevelError >= kLevel)
            logger::instance().log(func, line, kLevelError, format, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void fatal(const char* func, int line, const T& format, Args&&... args)
    {
        if constexpr (kLevelFatal >= kLevel)
            logger::instance().log(func, line, kLevelFatal, format, std::forward<Args>(args)...);
    }
}

#if EGGCPT_COMPILER_MSVC
#  ifndef __PRETTY_FUNCTION__
#    define __PRETTY_FUNCTION__ __FUNCSIG__
#  endif
#endif

#define EGGCPT_LOG(func, ...) eggcpt::logging::func(__PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)

#if EGGCPT_LOG_LEVEL <= 0
#  define EGGCPT_LOG_DEBUG(...) EGGCPT_LOG(debug, __VA_ARGS__)
#else
#  define EGGCPT_LOG_DEBUG(...) static_cast<void>(0)
#endif

#if EGGCPT_LOG_LEVEL <= 1
#  define EGGCPT_LOG_INFO(...) EGGCPT_LOG(info, __VA_ARGS__)
#else
#  define EGGCPT_LOG_INFO(...) static_cast<void>(0)
#endif

#if EGGCPT_LOG_LEVEL <= 2
#  define EGGCPT_LOG_WARN(...) EGGCPT_LOG(warn, __VA_ARGS__)
#else
#  define EGGCPT_LOG_WARN(...) static_cast<void>(0)
#endif

#if EGGCPT_LOG_LEVEL <= 3
#  define EGGCPT_LOG_ERROR(...) EGGCPT_LOG(error, __VA_ARGS__)
#else
#  define EGGCPT_LOG_ERROR(...) static_cast<void>(0)
#endif

#if EGGCPT_LOG_LEVEL <= 4
#  define EGGCPT_LOG_FATAL(...) EGGCPT_LOG(fatal, __VA_ARGS__)
#else
#  define EGGCPT_LOG_FATAL(...) static_cast<void>(0)
#endif
