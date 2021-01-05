#pragma once

#include <limits>
#include <optional>
#include <sstream>
#include <string>

#include <shell/algorithm.h>

namespace shell
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
inline std::optional<std::string> parse(const std::string& data)
{
    return data;
}

template<>
inline std::optional<bool> parse(const std::string& data)
{
    const std::string value = toLowerCopy(data);

    if (value == "1" || value == "true")  return true;
    if (value == "0" || value == "false") return false;

    return std::nullopt;
}

template<>
inline std::optional<int> parse(const std::string& data)
{
    try
    {
        return std::stoi(data);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<long> parse(const std::string& data)
{
    try
    {
        return std::stol(data);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<unsigned long> parse(const std::string& data)
{
    try
    {
        return std::stoul(data);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<long long> parse(const std::string& data)
{
    try
    {
        return std::stoll(data);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<unsigned long long> parse(const std::string& data)
{
    try
    {
        return std::stoull(data);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<float> parse(const std::string& data)
{
    try
    {
        return std::stof(data);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<double> parse(const std::string& data)
{
    try
    {
        return std::stod(data);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<unsigned int> parse(const std::string& data)
{
    if (const auto value = parse<long long>(data))
    {
        if (*value >= 0 && *value <= std::numeric_limits<unsigned int>::max())
            return static_cast<unsigned int>(*value);
    }
    return std::nullopt;
}

}  // namespace shell
