#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include "traits.h"

namespace eggcpt
{

namespace utility
{

template<typename T, typename... Args>
inline void reconstruct(T& instance, Args&&... args)
{
    static_assert(std::is_constructible_v<T, Args...>);

    instance.~T();
    new(&instance)T{ std::forward<Args>(args)... };
}

template<typename Target, typename Char>
inline std::optional<Target> string_to(const std::basic_string<Char>& data)
{
    static_assert(is_any_of_v<Target, int, long, long long, unsigned long, unsigned long long, float, double, long double>);
    static_assert(is_any_of_v<Char, char, wchar_t>);

    try
    {
        if constexpr (std::is_same_v<Target, int>) return std::stoi(data);
        if constexpr (std::is_same_v<Target, long>) return std::stol(data);
        if constexpr (std::is_same_v<Target, long long>) return std::stoll(data);
        if constexpr (std::is_same_v<Target, unsigned long>) return std::stoul(data);
        if constexpr (std::is_same_v<Target, unsigned long long>) return std::stoull(data);
        if constexpr (std::is_same_v<Target, float>) return std::stof(data);
        if constexpr (std::is_same_v<Target, double>) return std::stod(data);
        if constexpr (std::is_same_v<Target, long double>) return std::stold(data);
    } 
    catch (const std::out_of_range&) { return std::nullopt; }
    catch (const std::invalid_argument&) { return std::nullopt; }
}

}  // namespace utility

using utility::reconstruct;
using utility::string_to;

}  // namespace eggcpt
