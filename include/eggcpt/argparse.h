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

template<typename... Ts>
class Value
{
public:
    using Variant = std::variant<std::optional<Ts>...>;

    template<typename T>
    explicit Value(const std::optional<T>& value)
        : variant(value) {}

    template<typename T>
    explicit Value(const T& value)
        : Value(std::optional<T>(value)) {}

    Variant variant;
};

template<typename T>
std::optional<T> convert(const std::string& data)
{
    return string_to<T>(data);
}

template<>
std::optional<bool> convert(const std::string& data)
{
    return data.empty() || data == "true";
}

template<>
std::optional<std::string> convert(const std::string& data)
{
    return data;
}

class BasicConverter
{
public:
    BasicConverter(const std::string& data)
        : data(data) {}

    template<typename T>
    void operator()(std::optional<T>& value)
    {
        value = convert<T>(data);
    }

protected:
    const std::string& data;
};

template<typename Value = Value<bool, int, double, std::string>>
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
                    std::visit(BasicConverter(argv[i++]), value->variant);
                }
                else
                {
                    // Booleans can be enabled by just passing the key
                    if (std::holds_alternative<std::optional<bool>>(value->variant))
                        value->variant = std::optional(true);
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
            return *std::get<std::optional<T>>(value->variant);

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
