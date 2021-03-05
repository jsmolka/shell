#pragma once

#include <functional>
#include <type_traits>
#include <utility>

#include <shell/macros.h>

namespace shell
{

template<typename T, typename Function>
class Delegate
{
public:
    static_assert(std::is_member_function_pointer_v<Function>);

    Delegate() = default;

    Delegate(T& self, Function func)
        : self(&self), func(func) {}

    template<typename... Args>
    auto operator()(Args&&... args)
    {
        SHELL_ASSERT(self);
        SHELL_ASSERT(func);

        return std::invoke(func, self, std::forward<Args>(args)...);
    }

    T* self = nullptr;
    Function func = nullptr;
};

}  // namespace shell
