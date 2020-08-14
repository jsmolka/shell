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

template<typename T>
std::optional<T> convert(const std::string& data)
{
    if constexpr (std::is_same_v<T, std::string>)
        return data;
    else if constexpr (std::is_same_v<T, bool>)
        return data != "false" && data != "0";
    else
        return string_to<T>(data);
}

template<typename... Ts>
class BasicArgumentParser
{
public:
    template<typename T>
    void add(
        const std::vector<std::string>& keys,
        const std::string& description,
        const std::optional<T>& value)
    {
        for (const auto& key : keys)
        {
            if (find(key))
                throw std::invalid_argument("Duplicate key");
        }
        args.push_back({ keys, description, value });
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
                    std::visit(Converter{argv[i++]}, *value);
                }
                else
                {
                    // Booleans can be enabled by just passing the key
                    if (std::holds_alternative<std::optional<bool>>(*value))
                        *value = std::optional(true);
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
            return *std::get<std::optional<T>>(*value);

        throw std::invalid_argument("Invalid key");
    }

private:
    using Value = std::variant<std::optional<Ts>...>;

    struct Argument
    {
        std::vector<std::string> keys;
        std::string description;
        Value value;
    };

    struct Converter
    {
        template<typename T>
        void operator()(std::optional<T>& value)
        {
            value = convert<T>(data);
        }

        std::string data;
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

using ArgumentParser = BasicArgumentParser<bool, int, long, long long, unsigned long, unsigned long long, float, double, long double, std::string>;

}  // namespace argparse

using argparse::BasicArgumentParser;
using argparse::ArgumentParser;

}  // namespace eggcpt
