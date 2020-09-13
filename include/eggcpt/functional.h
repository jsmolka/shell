#pragma once

#include <functional>

namespace eggcpt
{

template<typename T>
constexpr T&& identity(T&& value)
{
    return std::forward<T>(value);
}

}  // namespace eggcpt
