#pragma once

#include <limits>
#include <optional>
#include <sstream>
#include <string>

#include <shell/algorithm.h>

namespace shell
{

namespace detail
{

class Validator
{
public:
    Validator(const std::string& data)
        : _data(data) {}

    operator bool() const
    {
        return _index == _data.size();
    }

    template<typename Predicate>
    void one(Predicate pred)
    {
        if (_index < _data.size() && pred(_data[_index]))
            ++_index;
    }

    template<typename Predicate>
    void all(Predicate pred)
    {
        while (_index < _data.size() && pred(_data[_index]))
            ++_index;
    }

private:
    std::size_t _index = 0;
    const std::string& _data;
};

template<typename T>
class IsSignChar
{
public:
    bool operator()(char ch) const
    {
        return ch == '+' || (std::is_signed_v<T> && ch == '-');
    }
};

class IsNumericChar
{
public:
    IsNumericChar(int base)
        : _base(base) {}

    bool operator()(char ch) const
    {
        if (_base <= 10)
        {
            return (ch >= '0') && (ch < ('0' + _base));
        }
        else if (_base <= 36)
        {
            return ((ch >= '0') && (ch <= '9'))
                || ((ch >= 'a') && (ch < ('a' + (_base - 10))));
        }
        return false;
    }

private:
    int _base;
};

class IsDecimalPointChar
{
public:
    bool operator()(char ch) const
    {
        return ch == '.';
    }
};

class IsExponentChar
{
public:
    bool operator()(char ch) const
    {
        return ch == 'e';
    }
};

template<typename T>
std::optional<T> parseInt(T(*parse)(const std::string&, std::size_t*, int), std::string data)
{
    trim(data);
    toLower(data);

    int base = 10;
    if (startsWith(data, "0b") || startsWith(data, "+0b") || startsWith(data, "-0b")) { base =  2; replaceFirst(data, "0b", ""); }
    if (startsWith(data, "0x") || startsWith(data, "+0x") || startsWith(data, "-0x")) { base = 16; replaceFirst(data, "0x", ""); }

    Validator validator(data);
    validator.one(IsSignChar<T>());
    validator.all(IsNumericChar(base));

    if (!validator)
        return std::nullopt;

    try
    {
        return parse(data, nullptr, base);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

template<typename T>
std::optional<T> parseRat(T(*parse)(const std::string&, std::size_t*), std::string data)
{
    trim(data);
    toLower(data);

    Validator validator(data);
    validator.one(IsSignChar<T>());
    validator.all(IsNumericChar(10));
    validator.one(IsDecimalPointChar());
    validator.all(IsNumericChar(10));
    validator.one(IsExponentChar());
    validator.one(IsSignChar<T>());
    validator.all(IsNumericChar(10));

    if (!validator)
        return std::nullopt;

    try
    {
        return parse(data, nullptr);
    }
    catch (const std::logic_error&)
    {
        return std::nullopt;
    }
}

}  // namespace detail

template<typename T>
std::optional<T> parse(const std::string& data)
{
    T value{};
    std::stringstream stream(data);
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
    std::string value = toLowerCopy(data);

    if (value == "1" || value == "true")  return true;
    if (value == "0" || value == "false") return false;

    return std::nullopt;
}

template<>
inline std::optional<int> parse(const std::string& data)
{
    return detail::parseInt<int>(std::stoi, data);
}

template<>
inline std::optional<long> parse(const std::string& data)
{
    return detail::parseInt<long>(std::stol, data);
}

template<>
inline std::optional<unsigned long> parse(const std::string& data)
{
    return detail::parseInt<unsigned long>(std::stoul, data);
}

template<>
inline std::optional<long long> parse(const std::string& data)
{
    return detail::parseInt<long long>(std::stoll, data);
}

template<>
inline std::optional<unsigned long long> parse(const std::string& data)
{
    return detail::parseInt<unsigned long long>(std::stoull, data);
}

template<>
inline std::optional<unsigned int> parse(const std::string& data)
{
    if (const auto value = parse<unsigned long>(data))
    {
        if (*value <= std::numeric_limits<unsigned int>::max())
            return static_cast<unsigned int>(*value);
    }
    return std::nullopt;
}

template<>
inline std::optional<float> parse(const std::string& data)
{
    return detail::parseRat<float>(std::stof, data);
}

template<>
inline std::optional<double> parse(const std::string& data)
{
    return detail::parseRat<double>(std::stod, data);
}

}  // namespace shell
