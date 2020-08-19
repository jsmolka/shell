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

class OptionError : public std::runtime_error
{
public:
    OptionError(const std::string& what)
        : std::runtime_error(what) {}
};

class InvalidValueOptionError : public OptionError
{
public:
    InvalidValueOptionError(const std::string& value)
        : OptionError(fmt::format("Invalid option value \"{}\"", value)) {}
};

class NoValueOptionError : public OptionError
{
public:
    NoValueOptionError()
        : OptionError("No value for non-bool option.") {}
};

class NotFoundOptionError : public OptionError
{
public:
    NotFoundOptionError()
        : OptionError("") {}
};

class Value : public std::enable_shared_from_this<Value>
{
public:
    virtual ~Value() = default;

    virtual void parse() = 0;
    virtual void parse(const std::string& data) = 0;
    virtual bool hasValue() const = 0;
    
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

    bool is_optional = false;
    bool is_positional = false;
};

template<typename T>
class AbstractValue : public Value
{
public:
    AbstractValue()
        : value(std::nullopt) {}

    AbstractValue(const T& value)
        : value(value) {}

    bool hasValue() const override
    {
        return value.has_value();
    }

    std::optional<T> value;
};

template<typename T>
class StandardValue : public AbstractValue<T>
{
public:
    using AbstractValue<T>::AbstractValue;

    void parse() override
    {
        throw NoValueOptionError();
    }

    void parse(const std::string& data) override
    {
        T result;

        std::stringstream stream;
        stream << data;
        stream >> result;

        if (!stream)
            throw InvalidValueOptionError(data);

        // Remove this? (Linux)
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
        if (data == "1" || data == "true")
        {
            value = true;
            return;
        }
        if (data == "0" || data == "false")
        {
            value = false;
            return;
        }
        throw InvalidValueOptionError(data);
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
            throw NotFoundOptionError();

        return *static_cast<StandardValue<T>*>(entry->second.get())->value;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Value>> options;
};

class Options
{
public:
    Options(const std::string& program, const std::string& desc)
        : program(program), desc(desc) {}

    template<typename T, typename... Args>
    static std::shared_ptr<Value> value(Args&&... args)
    {
        return std::make_shared<StandardValue<T>>(std::forward<Args>(args)...);
    }

    void add(const std::vector<std::string>& options, const std::string& desc, std::shared_ptr<Value> value)
    {
        Option option = { options, desc, value };

        // Todo: check duplicate

        if (value->is_positional)
            positional.push_back(option);
        else
            named.push_back(option);
    }

    OptionsResult parse(int argc, const char* argv[])
    {
        int pos = 1;
        int idx = 0;

        while (pos < argc)
        {
            auto key = argv[pos++];

            if (auto value = find(key))
            {
                if (pos < argc && !find(argv[pos]))
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

        // Verify first

        return makeResult();
    }

private:
    struct Option
    {
        std::vector<std::string> keys;
        std::string desc;
        std::shared_ptr<Value> value;
    };

    std::shared_ptr<Value> find(const std::string& key)
    {
        for (const auto& option : named)
        {
            if (contains(option.keys, key))
                return option.value;
        }
        return nullptr;
    }

    static void fillResult(OptionsResult& result, const std::vector<Option>& options)
    {
        for (const auto& option : options)
        {
            if (!option.value->hasValue())
                continue;

            for (const auto& key : option.keys)
                result.options[key] = option.value;
        }
    }

    OptionsResult makeResult() const
    {
        OptionsResult result;

        fillResult(result, named);
        fillResult(result, positional);

        return result;
    }

    std::string program;
    std::string desc;

    std::vector<Option> named;
    std::vector<Option> positional;
};

}  // namespace eggcpt
