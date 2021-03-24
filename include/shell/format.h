#pragma once

#include <shell/fmt.h>

namespace shell
{

template<typename... Args>
auto format(Args&&... args)
{
    static_assert(sizeof...(Args) > 0);

    if constexpr (sizeof...(Args) == 1)
        return fmt::format("{}", std::forward<Args>(args)...);
    else
        return fmt::format(std::forward<Args>(args)...);
}

template<typename... Args>
void print(Args&&... args)
{
    static_assert(sizeof...(Args) > 0);

    #ifndef SHELL_NO_CONSOLE
    if constexpr (sizeof...(Args) == 1)
        fmt::print("{}", std::forward<Args>(args)...);
    else
        fmt::print(std::forward<Args>(args)...);
    #endif
}

}  // namespace shell
