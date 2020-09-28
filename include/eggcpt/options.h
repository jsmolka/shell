#pragma once

#include <memory>
#include <vector>

#include <eggcpt/algorithm.h>
#include <eggcpt/errors.h>
#include <eggcpt/parse.h>

namespace eggcpt
{

namespace detail
{

class Value : public std::enable_shared_from_this<Value>
{
public:
    using Shared = std::shared_ptr<Value>;

    virtual ~Value() = default;

    Shared optional()
    {
        is_optional = true;
        return shared_from_this();
    }

    bool isOptional() const
    {
        return is_optional;
    }

    Shared positional()
    {
        is_positional = true;
        return shared_from_this();
    }

    bool isPositional() const
    {
        return is_positional;
    }

    virtual void parse() = 0;
    virtual void parse(const std::string& data) = 0;

    virtual bool hasValue() const = 0;
    virtual bool hasDefaultValue() const = 0;

    virtual std::string info() const = 0;

protected:
    bool is_optional = false;
    bool is_positional = false;
};

template<typename T>
class OptionValue : public Value
{
public:
    OptionValue() = default;

    explicit OptionValue(const T& value)
        : value(value)
        , default_value(value)
    {
        is_optional = true;
    }

    void parse() override
    {
        if constexpr (std::is_same_v<T, bool>)
            value = true;
        else
            throw ParseError("Expected value for non-bool option but got none");
    }

    void parse(const std::string& data) override
    {
        const auto optional = eggcpt::parse<T>(data);

        throwIf<ParseError>(!optional.has_value(), "Expected valid data but got '{}'", data);

        value = *optional;
    }

    bool hasValue() const override
    {
        return value.has_value();
    }

    bool hasDefaultValue() const override
    {
        return default_value.has_value();
    }

    std::string info() const override
    {
        if (hasDefaultValue())
            return fmt::format(" (default: {})", *default_value);

        if (isOptional())
            return fmt::format(" (optional)");

        return std::string();
    }

    std::optional<T> value;

private:
    std::optional<T> default_value;
};

struct Option
{
    std::vector<std::string> keys;
    Value::Shared value;
    std::string desc;
};

class OptionVector : public std::vector<Option>
{
public:
    Value::Shared find(const std::string& key) const
    {
        for (const auto& [keys, value, desc] : *this)
        {
            if (std::find(keys.begin(), keys.end(), key) != keys.end())
                return value;
        }
        return nullptr;
    }

    bool has(const std::string& key) const
    {
        return static_cast<bool>(find(key));
    }
};

class OptionGroup
{
public:
    explicit OptionGroup(const std::string& name)
        : name(name) {}

    std::string args() const
    {
        std::string result;
        for (const auto& [keys, value, desc] : options)
            result.append(fmt::format(value->isOptional() ? " [{}]" : " {}", keys.front()));

        return result;
    }

    std::string help() const
    {
        if (options.empty())
            return std::string();

        std::string result = fmt::format("\n{} arguments:\n", name);
        std::size_t padding = this->padding();

        for (const auto& [keys, value, desc] : options)
        {
            result.append(fmt::format(
                "{:<{}}{}{}\n",
                join(keys, kDelimiter),
                padding,
                desc,
                value->info()));
        }
        return result;
    }

    OptionVector options;

private:
    static constexpr std::string_view kDelimiter = ", ";

    std::size_t padding() const
    {
        std::size_t padding = 0;
        for (const auto& option : options)
        {
            std::size_t size = 2;
            for (const auto& key : option.keys)
                size += key.size() + kDelimiter.size();

            padding = std::max(size, padding);
        }
        return padding;
    }

    std::string name;
};

}  // namespace detail

class OptionsResult
{
public:
    friend class Options;

    bool has(const std::string& key) const
    {
        return options.has(key);
    }

    template<typename T>
    std::optional<T> find(const std::string& key) const
    {
        if (const auto value = options.find(key))
            return std::static_pointer_cast<detail::OptionValue<T>>(value)->value;

        return std::nullopt;
    }

    template<typename T>
    T findOr(const std::string& key, const T& fallback)
    {
        return find<T>(key).value_or(fallback);
    }

private:
    void populate(detail::OptionVector& other)
    {
        for (const auto& [keys, value, desc] : other)
        {
            if (value->hasValue())
                options.push_back({ keys, value, desc });
            else
                throwIf<ParseError>(!value->isOptional(), "Expected value for option '{}' but got none", keys.front());
        }
    }

    detail::OptionVector options;
};

class Options
{
public:
    explicit Options(const std::string& program)
        : program(program)
        , keyword("Keyword")
        , positional("Positional") {}

    template<typename T>
    static detail::Value::Shared value()
    {
        return std::make_shared<detail::OptionValue<T>>();
    }

    template<typename T>
    static detail::Value::Shared value(const T& value)
    {
        return std::make_shared<detail::OptionValue<T>>(value);
    }

    void add(const std::vector<std::string>& keys, const std::string& desc, detail::Value::Shared value)
    {
        auto& options = value->isPositional()
            ? positional.options
            : keyword.options;

        options.push_back({ keys, value, desc });
    }

    OptionsResult parse(int argc, char* argv[])
    {
        int pos = 1;
        int idx = 0;

        while (pos < argc)
        {
            std::string key = argv[pos++];

            if (auto value = keyword.options.find(key))
            {
                if (pos < argc && !keyword.options.has(argv[pos]))
                    value->parse(argv[pos++]);
                else
                    value->parse();
            }
            else
            {
                if (idx < positional.options.size())
                    positional.options[idx++].value->parse(key);
            }
        }

        OptionsResult result;
        result.populate(keyword.options);
        result.populate(positional.options);

        return result;
    }

    std::string help() const
    {
        return fmt::format(
            "Usage: {}{}{}\n{}{}",
            program,
            keyword.args(),
            positional.args(),
            keyword.help(),
            positional.help());
    }

private:
    std::string program;
    detail::OptionGroup keyword;
    detail::OptionGroup positional;
};

}  // namespace eggcpt
