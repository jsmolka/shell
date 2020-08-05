#pragma once

#include <optional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "algorithm.h"
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

    Value(const std::optional<bool>& data) : data(data), type(Type::Bool) {}
    Value(const std::optional<int>& data) : data(data), type(Type::Integer) {}
    Value(const std::optional<double>& data) : data(data), type(Type::Decimal) {}
    Value(const std::optional<std::string>& data) : data(data), type(Type::String) {}

    Type type;
    OptionalVariant<bool, int, double, std::string> data;
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
    void add(const std::vector<std::string>& keys, const std::string& description, const std::optional<T>& data)
    {
        args.push_back({ keys, description, Value(data) });
        //throw std::invalid_argument("duplicate key");  ???
    }

    void parse(int argc, const char* argv[])
    {
        int i = 0;
        while (i < argc)
        {
            std::string key(argv[i++]);

            if (auto value = find(key))
            {
                if (i < argc && !find(argv[i]))
                {
                    switch (value->type)
                    {
                    case Value::Type::Bool: value->data = Converter<bool>()(argv[i++]); break;
                    case Value::Type::Integer: value->data = Converter<int>()(argv[i++]); break;
                    case Value::Type::Decimal: value->data = Converter<double>()(argv[i++]); break;
                    case Value::Type::String: value->data = Converter<std::string>()(argv[i++]); break;
                    }
                }
                else
                {
                    if (value->type == Value::Type::Bool)
                        value->data = std::optional(true);
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
            return *std::get<std::optional<T>>(value->data);

        throw std::out_of_range("Invalid key");
    }

private:
    struct Argument
    {
        std::vector<std::string> keys;
        std::string description;
        Value value;
    };

    Value* find(const std::string& key)
    {
        for (auto& arg : args)
        {
            if (contains(arg.keys, key))
                return &arg.value;
        }
        return nullptr;
    }

    std::vector<Argument> args;
    std::vector<std::string> unbound;
};

}  // namespace argparse

using argparse::ArgumentParser;

}  // namespace eggcpt
