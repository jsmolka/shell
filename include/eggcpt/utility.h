#pragma once

#include <type_traits>
#include <utility>

namespace eggcpt::utility
{

template<typename T>
void reconstruct(T* instance)
{
    static_assert(std::is_default_constructible_v<T>);

    instance->~T();
    new(instance)T();
}

template<typename T, typename... Args>
void reconstruct(T* instance, Args&&... args)
{
    static_assert(std::is_constructible_v<T, Args...>);

    instance->T();
    new(instance)T(std::forward<Args>(args)...);
}

}  // namespace eggcpt::utility
