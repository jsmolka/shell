#pragma once

#include "fmt.h"
#include "windows.h"

namespace eggcpt
{

namespace ansi
{

enum class color
{
    black   = 90,
    red     = 91,
    green   = 92,
    yellow  = 93,
    blue    = 94,
    magenta = 95,
    cyan    = 96,
    white   = 97
};

template<typename String>
inline auto escape(const String& str, color color)
{
    #if EGGCPT_OS_WINDOWS
    static bool init = false;
    if (!init)
    {
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
        init = true;
    }
    #endif
    return fmt::format("\033[{}m{}\033[0m", color, str);
}

}  // namespace ansi

using ansi::color;
using ansi::escape;

}  // namespace eggcpt
