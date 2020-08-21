#pragma once

#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "algorithm.h"
#include "errors.h"

namespace eggcpt
{

class OptionError : public FormattedError
{
public:
    using FormattedError::FormattedError;
};

namespace detail
{

class Value : public std::enable_shared_from_this<Value>
{
public:
    using Pointer = std::shared_ptr<Value>;

    explicit Value(bool optional)
        : is_optional(optional)
        , is_positional(false) {}

    virtual ~Value() = default;

    Pointer optional()
    {
        is_optional = true;
        return shared_from_this();
    }

    Pointer positional()
    {
        is_positional = true;
        return shared_from_this();
    }

    virtual void parse() = 0;
    virtual void parse(const std::string& data) = 0;
    virtual bool hasValue() const = 0;
    virtual bool hasDefaultValue() const = 0;
    virtual std::string getValue() const = 0;
    virtual std::string getDefaultValue() const = 0;

    bool is_optional;
    bool is_positional;
};

template<typename T>
class GenericValue : public Value
{
public:
    GenericValue()
        : Value(false)
        , value(std::nullopt)
        , default_value(std::nullopt) {}

    explicit GenericValue(const T& value)
        : Value(true)
        , value(value)
        , default_value(value) {}

    bool hasValue() const override
    {
        return static_cast<bool>(value);
    }

    bool hasDefaultValue() const override
    {
        return static_cast<bool>(default_value);
    }

    std::string getValue() const override
    {
        std::stringstream stream;
        stream << std::boolalpha;
        stream << *value;

        return stream.str();
    }

    std::string getDefaultValue() const override
    {
        std::stringstream stream;
        stream << std::boolalpha;
        stream << *default_value;

        return stream.str();
    }

    std::optional<T> value;

private:
    std::optional<T> default_value;
};

template<typename T>
class OptionValue : public GenericValue<T>
{
public:
    using GenericValue<T>::GenericValue;

    void parse() override
    {
        throw OptionError("Missing value for non-bool option");
    }

    void parse(const std::string& data) override
    {
        T result;

        std::stringstream stream;
        stream << data;
        stream >> result;

        if (!stream)
            throw OptionError("Invalid data: {}", data);

        this->value = result;
    }
};

template<>
class OptionValue<bool> : public GenericValue<bool>
{
public:
    using GenericValue<bool>::GenericValue;

    void parse() override
    {
        value = true;
    }

    void parse(const std::string& data) override
    {
             if (data == "1" || data == "true" ) value = true;
        else if (data == "0" || data == "false") value = false;
        else throw OptionError("Invalid bool data: {}", data);
    }
};

struct Option
{
    std::vector<std::string> keys;
    std::string description;
    Value::Pointer value;
};

inline Value::Pointer find(const std::vector<Option>& options, const std::string& key)
{
    for (const auto& [keys, desc, value] : options)
    {
        if (std::find(keys.begin(), keys.end(), key) != keys.end())
            return value;
    }
    return nullptr;
}

}  // namespace detail

class OptionsResult
{
public:
    friend class Options;

    bool has(const std::string& key) const
    {
        return static_cast<bool>(detail::find(options, key));
    }

    template<typename T>
    T get(const std::string& key) const
    {
        if (auto value = detail::find(options, key))
            return *static_cast<detail::OptionValue<T>&>(*value).value;

        throw OptionError("Invalid key: {}", key);
    }

private:
    void fill(const std::vector<detail::Option>& options)
    {
        for (const auto& [keys, desc, value] : options)
        {
            if (!value->hasValue())
            {
                if (!value->is_optional)
                    throw OptionError("Missing option: {}", join(keys, ", "));

                continue;
            }
            this->options.push_back({ keys, desc, value });
        }
    }

    std::vector<detail::Option> options;
};

class Options
{
public:
    explicit Options(const std::string& program)
        : program(program) {}

    template<typename T, typename... Args>
    static detail::Value::Pointer value(Args&&... args)
    {
        return std::make_shared<detail::OptionValue<T>>(std::forward<Args>(args)...);
    }

    void add(const std::vector<std::string>& keys, const std::string& desc, detail::Value::Pointer value)
    {
        if (keys.empty())
            throw OptionError("Empty keys");

        for (const auto& key : keys)
        {
            if (key.empty())
                throw OptionError("Empty key");

            if (detail::find(keyword, key) || detail::find(positional, key))
                throw OptionError(key);
        }
        (value->is_positional ? positional : keyword).push_back({ keys, desc, value });
    }

    OptionsResult parse(int argc, const char* argv[])
    {
        int pos = 1;
        int idx = 0;

        while (pos < argc)
        {
            auto key = argv[pos++];

            if (auto value = detail::find(keyword, key))
            {
                if (pos < argc && !detail::find(keyword, argv[pos]))
                    value->parse(argv[pos++]);
                else
                    value->parse();
            }
            else
            {
                if (idx < positional.size())
                    positional[idx++].value->parse(key);
            }
        }

        OptionsResult result;
        result.fill(keyword);
        result.fill(positional);

        return result;
    }

    std::string help() const
    {
        std::stringstream stream;
        stream << format("Usage: {}", program);

        for (const auto& options : { keyword, positional })
        {
            for (const auto& [keys, desc, value] : options)
                stream << format(value->is_optional ? " [{}]" : " {}", keys.front());
        }
        stream << std::endl;

        for (const auto& [kind, options] : { Group{ "Keyword", keyword }, Group{ "Positional", positional } })
        {
            stream << format("\n{} arguments\n", kind);

            std::size_t padding = 0;
            for (const auto& [keys, desc, value] : options)
            {
                std::size_t size = 0;
                for (const auto& key : keys)
                    size += key.size() + 2;

                padding = std::max(size, padding);
            }

            for (const auto& [keys, desc, value] : options)
            {
                auto key = join(keys, ", ");
                auto def = value->hasDefaultValue()
                    ? format(" (default: {})", value->getDefaultValue())
                    : "";

                stream << format("{:<{}}{}{}\n", key, padding, desc, def);
            }
        }
        return stream.str();
    }

private:
    struct Group
    {
        std::string kind;
        std::vector<detail::Option> options;
    };

    std::string program;
    std::vector<detail::Option> keyword;
    std::vector<detail::Option> positional;
};

}  // namespace eggcpt
