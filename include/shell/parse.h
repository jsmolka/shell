#pragma once

#include <limits>
#include <optional>
#include <sstream>
#include <string>

#include <shell/algorithm.h>

namespace shell
{

template<typename T>
std::optional<T> parseInt(const std::string& data, int base = 10, std::size_t* index = nullptr);

template<>
inline std::optional<int> parseInt(const std::string& data, int base, std::size_t* index)
{
    try
    {
        return std::stoi(data, index, base);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<long> parseInt(const std::string& data, int base, std::size_t* index)
{
    try
    {
        return std::stol(data, index, base);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<unsigned long> parseInt(const std::string& data, int base, std::size_t* index)
{
    try
    {
        return std::stoul(data, index, base);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<long long> parseInt(const std::string& data, int base, std::size_t* index)
{
    try
    {
        return std::stoll(data, index, base);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<unsigned long long> parseInt(const std::string& data, int base, std::size_t* index)
{
    try
    {
        return std::stoull(data, index, base);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<unsigned int> parseInt(const std::string& data, int base, std::size_t* index)
{
    if (const auto value = parseInt<long long>(data, base, index))
    {
        if (*value >= 0 && *value <= std::numeric_limits<unsigned int>::max())
            return static_cast<unsigned int>(*value);
    }
    return std::nullopt;
}

template<typename T>
std::optional<T> parseRat(const std::string& data, std::size_t* index = nullptr);

template<>
inline std::optional<float> parseRat(const std::string& data, std::size_t* index)
{
    try
    {
        return std::stof(data, index);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<>
inline std::optional<double> parseRat(const std::string& data, std::size_t* index)
{
    try
    {
        return std::stod(data, index);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

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
    return parseInt<int>(data);
}

template<>
inline std::optional<long> parse(const std::string& data)
{
    return parseInt<long>(data);
}

template<>
inline std::optional<unsigned long> parse(const std::string& data)
{
    return parseInt<unsigned long>(data);
}

template<>
inline std::optional<long long> parse(const std::string& data)
{
    return parseInt<long long>(data);
}

template<>
inline std::optional<unsigned long long> parse(const std::string& data)
{
    return parseInt<unsigned long long>(data);
}

template<>
inline std::optional<unsigned int> parse(const std::string& data)
{
    return parseInt<unsigned int>(data);
}

template<>
inline std::optional<float> parse(const std::string& data)
{
    return parseRat<float>(data);
}

template<>
inline std::optional<double> parse(const std::string& data)
{
    return parseRat<double>(data);
}

}  // namespace shell
