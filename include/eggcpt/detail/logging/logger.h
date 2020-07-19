#pragma once

#include <string>
#include <eggcpt/detail/logging/level.h>
#include <eggcpt/detail/utility/singleton.h>
#include <eggcpt/detail/windows.h>
#include <eggcpt/fmt.h>

namespace eggcpt::logging
{

class logger : public utility::singleton<logger>
{
public:
    logger(token)
    {
        #if EGGCPT_PLATFORM_WINDOWS
        auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (handle != INVALID_HANDLE_VALUE)
        {
            DWORD mode;
            if (GetConsoleMode(handle, &mode))
            {
                mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(handle, mode);
            }
        }
        #endif
    }

    ~logger()
    {
        #if EGGCPT_PLATFORM_WINDOWS
        auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (handle != INVALID_HANDLE_VALUE)
        {
            DWORD mode;
            if (GetConsoleMode(handle, &mode))
            {
                mode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(handle, mode);
            }
        }
        #endif
    }

    template<typename T, typename... Args>
    void log(const char* func, int line, int level, const T& format, Args&&... args)
    {
        std::string prefix;
        std::string message = fmt::format(format, std::forward<Args>(args)...);

        switch (level)
        {
        case kLevelDebug: prefix = "\033[96m[D]"; break;
        case kLevelInfo:  prefix = "\033[92m[I]"; break;
        case kLevelWarn:  prefix = "\033[93m[W]"; break;
        case kLevelError: prefix = "\033[91m[E]"; break;
        case kLevelFatal: prefix = "\033[95m[F]"; break;
        }

        fmt::print("{} {}:{} - {}\033[0m\n", prefix, func, line, message);
    }
};

}  // namespace eggcpt::logging
