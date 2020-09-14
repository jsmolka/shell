#pragma once

#include <optional>
#include <sstream>
#include <string>

#include <eggcpt/filesystem.h>

namespace eggcpt
{

template<typename T>
std::optional<T> parse(const std::string& data)
{
    T value{};
    std::stringstream stream(data);
    stream << std::boolalpha;
    stream >> value;

    return stream
        ? std::optional(value)
        : std::nullopt;
}

template<>
std::optional<std::string> parse(const std::string& data)
{
    return data;
}

template<>
std::optional<filesystem::path> parse(const std::string& data)
{
    return filesystem::u8path(data);
}

}  // namespace eggcpt
