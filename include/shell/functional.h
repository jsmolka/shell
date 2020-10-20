#pragma once

#include <utility>

namespace shell
{

namespace functional
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

}  // namespace functional

using functional::Contradiction;
using functional::Identity;
using functional::Tautology;

}  // namespace shell
