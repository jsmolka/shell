#pragma once

#include <utility>

namespace eggcpt
{

struct Identity
{
    template<typename T>
    constexpr T&& operator()(T&& value) const
    {
        return std::forward<T>(value);
    }
};

struct Tautology
{
    template<typename T>
    constexpr bool operator()(const T& value) const
    {
        return true;
    }
};

struct Contradiction
{
    template<typename T>
    constexpr bool operator()(const T& value) const
    {
        return false;
    }
};

}  // namespace eggcpt
