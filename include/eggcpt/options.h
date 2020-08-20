#pragma once

#include <optional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "algorithm.h"
#include "fmt.h"
#include "primitives.h"

namespace eggcpt
{

class OptionError : public std::exception
{
public:
    explicit OptionError(const std::string& message)
        : message(message) {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;
};

class OptionParseError : public OptionError
{
public:
    using OptionError::OptionError;
};

class OptionValueError : public OptionParseError
{
public:
    using OptionParseError::OptionParseError;
};

class OptionMissingError : public OptionParseError
{
public:
    explicit OptionMissingError(const std::vector<std::string>& keys)
        // Todo: Better implode -> join
        : OptionParseError(fmt::format("Required option missing: {}", implode<std::vector<std::string>, std::string>(keys, ","))) {}
};

class OptionDuplicateKeyError : public OptionError
{
public:
    explicit OptionDuplicateKeyError(const std::string& key)
        : OptionError(fmt::format("Duplicate key: {}", key)) {}
};

class OptionInvalidKeyError : public OptionError
{
public:
    explicit OptionInvalidKeyError(const std::string& key)
        : OptionError(fmt::format("Invalid key: {}", key)) {}
};

class Value : public std::enable_shared_from_this<Value>
{
public:
    explicit Value(bool optional)
        : is_optional(optional)
        , is_positional(false) {}

    virtual ~Value() = default;

    virtual void parse() = 0;
    virtual void parse(const std::string& data) = 0;
    virtual bool hasValue() const = 0;
    virtual bool hasDefaultValue() const = 0;
    virtual std::string defaultValue() const = 0;
    
    std::shared_ptr<Value> optional()
    {
        is_optional = true;
        return shared_from_this();
    }

    std::shared_ptr<Value> positional()
    {
        is_positional = true;
        return shared_from_this();
    }

    bool is_optional;
    bool is_positional;
};

template<typename T>
class AbstractValue : public Value
{
public:
    AbstractValue()
        : Value(false)
        , value(std::nullopt)
        , default_value(std::nullopt) {}

    AbstractValue(const T& value)
        : Value(true)
        , value(value)
        , default_value(value) {}

    bool hasValue() const override
    {
        return value.has_value();
    }

    bool hasDefaultValue() const override
    {
        return default_value.has_value();
    }

    std::string defaultValue() const override
    {
        std::string result;

        std::stringstream stream;
        stream << std::boolalpha;
        stream << *default_value;
        stream >> result;

        return result;
    }

    std::optional<T> value;
    std::optional<T> default_value;
};

template<typename T>
class StandardValue : public AbstractValue<T>
{
public:
    using AbstractValue<T>::AbstractValue;

    void parse() override
    {
        throw OptionValueError("No value for non-bool option");
    }

    void parse(const std::string& data) override
    {
        T result;

        std::stringstream stream;
        stream << data;
        stream >> result;

        if (!stream)
            throw OptionValueError(fmt::format("Invalid value: {}", data));

        this->value = result;
    }
};

template<>
class StandardValue<bool> : public AbstractValue<bool>
{
public:
    using AbstractValue<bool>::AbstractValue;

    void parse() override 
    {
        value = true;
    }

    void parse(const std::string& data) override
    {
        if (data == "1" || data == "true" ) { value = true;  return; }
        if (data == "0" || data == "false") { value = false; return; }

        throw OptionValueError(fmt::format("Invalid bool value: {}", data));
    }
};

class OptionsResult
{
public:
    friend class Options;

    bool has(const std::string& option) const
    {
        return options.find(option) != options.end();
    }

    template<typename T>
    T get(const std::string& key)
    {
        auto entry = options.find(key);
        if (entry == options.end())
            throw OptionInvalidKeyError(key);

        return *static_cast<StandardValue<T>*>(entry->second.get())->value;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Value>> options;
};

class Options
{
public:
    explicit Options(const std::string& program)
        : program(program) {}

    template<typename T, typename... Args>
    static std::shared_ptr<Value> value(Args&&... args)
    {
        return std::make_shared<StandardValue<T>>(std::forward<Args>(args)...);
    }

    void add(const std::vector<std::string>& keys, const std::string& desc, std::shared_ptr<Value> value)
    {
        if (keys.empty())
            return;

        for (const auto& key : keys)
        {
            if (find(keyword, key) || find(positional, key))
                throw OptionDuplicateKeyError(key);
        }

        Option option = { keys, desc, value };

        if (value->is_positional)
            positional.push_back(option);
        else
            keyword.push_back(option);
    }

    OptionsResult parse(int argc, const char* argv[])
    {
        int pos = 1;
        int idx = 0;

        while (pos < argc)
        {
            auto key = argv[pos++];

            if (auto value = find(keyword, key))
            {
                if (pos < argc && !find(keyword, argv[pos]))
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

        return makeResult();
    }

    std::string help() const
    {
        std::string help = fmt::format("Usage: {}", program);

        for (const auto& options : { keyword, positional })
        {
            for (const auto& [keys, desc, value] : options)
            {
                if (keys.empty())
                    continue;

                help.append(fmt::format(value->is_optional ? " [{}]" : " {}", keys.front()));
            }
        }
        help.append("\n");

        using Group = std::pair<std::string, std::vector<Option>>;

        for (const auto& [name, options] : { Group{ "Keyword arguments", keyword }, Group{ "Positional arguments", positional } })
        {
            help.append(fmt::format("\n{}\n", name));

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
                if (value->hasDefaultValue())
                {
                    help.append(fmt::format("{:<{}}{} (default: {})\n",
                        implode(keys, std::string(", ")),
                        padding,
                        desc,
                        value->defaultValue()));
                }
                else
                {
                    help.append(fmt::format("{:<{}}{}\n",
                        implode(keys, std::string(", ")),
                        padding,
                        desc));
                }

            }
        }

        return help;
    }

private:
    struct Option
    {
        std::vector<std::string> keys;
        std::string desc;
        std::shared_ptr<Value> value;
    };

    static std::shared_ptr<Value> find(const std::vector<Option>& options, const std::string& key)
    {
        for (const auto& [keys, desc, value] : options)
        {
            if (std::find(keys.begin(), keys.end(), key) != keys.end())
                return value;
        }
        return nullptr;
    }

    static void fillResult(OptionsResult& result, const std::vector<Option>& options)
    {
        for (const auto& [keys, desc, value] : options)
        {
            if (!value->hasValue())
            {
                if (!value->is_optional)
                    throw OptionMissingError(keys);

                continue;
            }

            for (const auto& key : keys)
                result.options[key] = value;
        }
    }

    OptionsResult makeResult() const
    {
        OptionsResult result;

        fillResult(result, keyword);
        fillResult(result, positional);

        return result;
    }

    std::string program;
    std::vector<Option> keyword;
    std::vector<Option> positional;
};

}  // namespace eggcpt
