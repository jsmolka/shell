#pragma once

#include <optional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "utility.h"

namespace eggcpt
{

namespace argparse
{

struct Value
{
    enum class Type { Bool, Integer, Decimal, String };

    template<typename... Ts>
    using OptionalVariant = std::variant<std::optional<Ts>...>;

    Value(const std::optional<bool>& value) : value(value), type(Type::Bool) {}
    Value(const std::optional<int>& value) : value(value), type(Type::Integer) {}
    Value(const std::optional<double>& value) : value(value), type(Type::Decimal) {}
    Value(const std::optional<std::string>& value) : value(value), type(Type::String) {}

    Type type;
    OptionalVariant<bool, int, double, std::string> value;
};

template<typename T>
struct Converter {};

template<>
struct Converter<bool>
{
    inline std::optional<bool> operator()(const std::string& data) const
    {
        return data.empty() || data == "true";
    }
};

template<>
struct Converter<int>
{
    inline std::optional<int> operator()(const std::string& data) const
    {
        return string_to<int>(data);
    }
};

template<>
struct Converter<double>
{
    inline std::optional<double> operator()(const std::string& data) const
    {
        return string_to<double>(data);
    }
};

template<>
struct Converter<std::string>
{
    inline std::optional<std::string> operator()(const std::string& data) const
    {
        return data;
    }
};


class ArgumentParser
{
public:
    template<typename T>
    void add(const std::vector<std::string>& keys, const std::string& description, const std::optional<T>& default)
    {
        auto value = std::make_shared<Value>(default);

        for (const auto& key : keys)
        {
            if (values.find(key) != values.end())
                throw std::invalid_argument("duplicate key");

            values[key] = value;
        }
    }

    void parse(int argc, char* argv[])
    {
        int i = 0;
        while (i < argc)
        {
            std::string key(argv[i++]);

            if (auto value = find(key))
            {
                if (i < argc && !isKey(argv[i]))
                {
                    switch ((*value)->type)
                    {
                    case Value::Type::Bool: (*value)->value = Converter<bool>()(argv[i++]); break;
                    case Value::Type::Integer: (*value)->value = Converter<int>()(argv[i++]); break;
                    case Value::Type::Decimal: (*value)->value = Converter<double>()(argv[i++]); break;
                    case Value::Type::String: (*value)->value = Converter<std::string>()(argv[i++]); break;
                    }
                }
                else
                {
                    if ((*value)->type == Value::Type::Bool)
                        (*value)->value = std::optional(true);
                }
            }
            else
            {
                unbound.push_back(key);
            }
        }
    }

    template<typename T>
    T get(const std::string& key)
    {
        if (auto value = find(key))
            return *std::get<std::optional<T>>((*value)->value);

        throw std::out_of_range("Invalid key");
    }

private:
    bool isKey(const std::string& key) const
    {
        return values.find(key) != values.end();
    }

    std::optional<std::shared_ptr<Value>> find(const std::string& key)
    {
        auto iter = values.find(key);

        if (iter != values.end())
            return iter->second;
        else
            return std::nullopt;
    }

    std::vector<std::string> unbound;
    std::unordered_map<std::string, std::shared_ptr<Value>> values;
};

}  // namespace argparse

using argparse::ArgumentParser;

}  // namespace eggcpt
