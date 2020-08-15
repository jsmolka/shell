#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include "primitives.h"
#include "traits.h"

namespace eggcpt
{

template<typename T, typename... Args>
void reconstruct(T& instance, Args&&... args)
{
    static_assert(std::is_constructible_v<T, Args...>);

    instance.~T();
    new(&instance)T(std::forward<Args>(args)...);
}

template<typename Primitive, typename Char>
std::optional<Primitive> parse_primitive(const std::basic_string<Char>& data)
{
    static_assert(is_any_of_v<Char, char, wchar_t>);
    static_assert(is_any_of_v<Primitive, s32, u32, s64, u64, f32, f64>);

    try
    {
        if constexpr (std::is_same_v<Primitive, s32>) return std::stoi(data);
        if constexpr (std::is_same_v<Primitive, u32>) return std::stoul(data);
        if constexpr (std::is_same_v<Primitive, s64>) return std::stoll(data);
        if constexpr (std::is_same_v<Primitive, u64>) return std::stoull(data);
        if constexpr (std::is_same_v<Primitive, f32>) return std::stof(data);
        if constexpr (std::is_same_v<Primitive, f64>) return std::stod(data);
    } 
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

}  // namespace eggcpt
