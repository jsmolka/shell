#pragma once

#include <memory>
#include <vector>

#include <shell/algorithm.h>
#include <shell/errors.h>
#include <shell/utility.h>

namespace shell
{

namespace options
{

class OptionSpec
{
public:
    friend class Options;

    OptionSpec(
            const std::string& opt1,
            const std::string& opt2 = std::string(),
            const std::string& name = std::string())
        : opt1(opt1), opt2(opt2), name(name)
    {
        throwIf<Error>(opt1.empty() && opt2.empty(), "Expected non-empty specification");
    }

    std::string option() const
    {
        return opt1.empty() ? opt2 : opt1;
    }

    std::string options() const
    {
        if (opt1.empty()) return opt2;
        if (opt2.empty()) return opt1;

        return fmt::format("{}, {}", opt1, opt2);
    }

    std::string argument() const
    {
        if (positional)
            return fmt::format("<{}>", option());

        return name.empty()
            ? fmt::format("{}", option())
            : fmt::format("{} <{}>", option(), name);
    }

    std::string opt1;
    std::string opt2;
    std::string name;

private:
    bool positional = false;
};

namespace detail
{

class Value : public std::enable_shared_from_this<Value>
{
public:
    using Pointer = std::shared_ptr<Value>;

    virtual ~Value() = default;

    bool isOptional() const
    {
        return is_optional;
    }

    Pointer optional()
    {
        is_optional = true;
        return shared_from_this();
    }

    bool isPositional() const
    {
        return is_positional;
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
    virtual bool isBoolValue() const = 0;
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
        const auto optional = shell::parse<T>(data);

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

    bool isBoolValue() const override
    {
        return std::is_same_v<T, bool>;
    }

    std::string info() const override
    {
        if (hasDefaultValue())
            return fmt::format(" (default: {})", *default_value);

        if (isOptional())
            return " (optional)";

        return std::string();
    }

    std::optional<T> value;

private:
    std::optional<T> default_value;
};

struct Option
{
    OptionSpec spec;
    Value::Pointer value;
    std::string desc;
};

class OptionVector : public std::vector<Option>
{
public:
    Value::Pointer find(const std::string& key) const
    {
        for (const auto& [spec, value, desc] : *this)
        {
            if (spec.opt1 == key || spec.opt2 == key)
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

    std::string arguments() const
    {
        std::string result;
        for (const auto& [spec, value, desc] : options)
            result.append(fmt::format(value->isOptional() ? " [{}]" : " {}", spec.argument()));

        return result;
    }

    std::string argumentsHelp() const
    {
        if (options.empty())
            return std::string();

        std::size_t size = maxOptionsSize();
        std::string result = fmt::format("\n{} arguments:\n", name);

        for (const auto& [spec, value, desc] : options)
        {
            result.append(fmt::format(
                "{:<{}}{}{}\n",
                spec.options(),
                size,
                desc,
                value->info()));
        }
        return result;
    }

    OptionVector options;

private:
    std::size_t maxOptionsSize() const
    {
        std::size_t padding = 0;
        for (const auto& [spec, value, dec] : options)
            padding = std::max(spec.options().size() + 2, padding);

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
        for (const auto& [spec, value, desc] : other)
        {
            if (value->hasValue())
                options.push_back({ spec, value, desc });
            else
                throwIf<ParseError>(!value->isOptional(), "Expected value for option '{}' but got none", spec.option());
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
    static detail::Value::Pointer value()
    {
        return std::make_shared<detail::OptionValue<T>>();
    }

    template<typename T>
    static detail::Value::Pointer value(const T& value)
    {
        return std::make_shared<detail::OptionValue<T>>(value);
    }

    void add(OptionSpec spec, const std::string& desc, detail::Value::Pointer value)
    {
        spec.positional = value->isPositional();

        auto& options = value->isPositional()
            ? positional.options
            : keyword.options;

        options.push_back({ spec, value, desc });
    }

    OptionsResult parse(int argc, char* argv[])
    {
        int pos = 1;
        int idx = 0;

        while (pos < argc)
        {
            const std::string key = argv[pos++];

            if (auto value = keyword.options.find(key))
            {
                if (pos < argc && !keyword.options.has(argv[pos]) && (!value->isBoolValue() || shell::parse<bool>(argv[pos])))
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
            keyword.arguments(),
            positional.arguments(),
            keyword.argumentsHelp(),
            positional.argumentsHelp());
    }

private:
    std::string program;
    detail::OptionGroup keyword;
    detail::OptionGroup positional;
};

}  // namespace options

using options::Options;
using options::OptionSpec;
using options::OptionsResult;

}  // namespace shell
