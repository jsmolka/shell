#pragma once

#include <memory>
#include <vector>

#include <shell/algorithm.h>
#include <shell/errors.h>
#include <shell/parse.h>

namespace shell
{

class OptionSpecError : public Error
{
public:
    using Error::Error;
};

class OptionSpec
{
public:
    friend class Options;

    OptionSpec(
            const std::string& opts,
            const std::string& desc = std::string(),
            const std::string& name = std::string())
        : opts(split(opts, ",")), desc(desc), name(name)
    {
        if (opts.empty()) throw OptionSpecError("Expected non-empty specification");
    }

    std::string options() const
    {
        return join(opts, ", ");
    }

    std::string argument() const
    {
        const std::string& option = opts.front();

        if (positional)
            return fmt::format("<{}>", option);

        return name.empty()
            ? fmt::format("{}", option)
            : fmt::format("{} <{}>", option, name);
    }

    std::vector<std::string> opts;
    std::string desc;
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

    virtual Pointer optional()
    {
        is_optional = true;
        return shared_from_this();
    }

    bool isPositional() const
    {
        return is_positional;
    }

    virtual Pointer positional()
    {
        is_positional = true;
        return shared_from_this();
    }
       
    virtual void parse() = 0;
    virtual void parse(const std::string& data) = 0;
    virtual bool hasValue() const = 0;
    virtual bool isBoolean() const = 0;
    virtual std::string help() const = 0;

protected:
    bool is_optional = false;
    bool is_positional = false;
};

template<typename T>
class BasicValue : public Value
{
public:
    void parse(const std::string& data) final
    {
        if (const auto value = shell::parse<T>(data))
            this->value = value;
        else
            throw ParseError("Expected valid data but got '{}'", data);
    }

    bool hasValue() const final
    {
        return value.has_value();
    }

    bool isBoolean() const final
    {
        return std::is_same_v<T, bool>;
    }

    std::string help() const final
    {
        if (default_value)
            return fmt::format(" (default: {})", *default_value);

        if (is_optional)
            return " (optional)";

        return std::string();
    }

    std::optional<T> value;

protected:
    std::optional<T> default_value;
};

template<typename T>
class OptionValue final : public BasicValue<T>
{
public:
    OptionValue() = default;

    explicit OptionValue(const T& value)
    {
        this->value = value;
        this->default_value = value;
        this->is_optional = true;
    }

    void parse() final
    {
        throw ParseError("Expected value for non-bool option but got none");
    }
};

template<>
class OptionValue<bool> final : public BasicValue<bool>
{
public:
    OptionValue()
    {
        this->value = false;
        this->default_value = false;
        this->is_optional = true;
    }

    Pointer positional() final
    {
        throw OptionSpecError("Bool options cannot be positional");
    }

    void parse() final
    {
        this->value = true;
    }
};

struct Option
{
    OptionSpec spec;
    Value::Pointer value;
};

class OptionVector : public std::vector<Option>
{
public:
    Value::Pointer find(const std::string& key) const
    {
        for (const auto& [spec, value] : *this)
        {
            if (contains(spec.opts, key))
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
        for (const auto& [spec, value] : options)
            result.append(fmt::format(value->isOptional() ? " [{}]" : " {}", spec.argument()));

        return result;
    }

    std::string help() const
    {
        if (options.empty())
            return std::string();

        std::size_t padding = this->padding();
        std::string result = fmt::format("\n{} arguments:\n", name);

        for (const auto& [spec, value] : options)
        {
            result.append(fmt::format(
                "{:<{}}{}{}\n",
                spec.options(),
                padding,
                spec.desc,
                value->help()));
        }
        return result;
    }

    OptionVector options;

private:
    std::size_t padding() const
    {
        std::size_t padding = 0;
        for (const auto& [spec, value] : options)
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
    void copy(const detail::OptionVector& vector)
    {
        for (const auto& [spec, value] : vector)
        {
            if (value->hasValue())
                options.push_back({ spec, value });
            else if (!value->isOptional())
                throw ParseError("Expected value for option '{}' but got none", spec.opts.front());
        }
    }

    detail::OptionVector options;
};

class Options
{
public:
    explicit Options(const std::string& program)
        : program(program)
        , group_key("Keyword")
        , group_pos("Positional") {}

    template<typename T>
    static detail::Value::Pointer value()
    {
        return std::make_shared<detail::OptionValue<T>>();
    }

    template<typename T>
    static detail::Value::Pointer value(const T& value)
    {
        static_assert(!std::is_same_v<T, bool>);

        return std::make_shared<detail::OptionValue<T>>(value);
    }

    void add(OptionSpec spec, detail::Value::Pointer value)
    {
        spec.positional = value->isPositional();

        auto& options = value->isPositional()
            ? group_pos.options
            : group_key.options;

        options.push_back({ spec, value });
    }

    OptionsResult parse(int argc, char* argv[])
    {
        int pos = 1;
        int idx = 0;

        while (pos < argc)
        {
            std::string key(argv[pos++]);

            if (auto value = group_key.options.find(key))
            {
                if (pos < argc && !group_key.options.has(argv[pos]) && !value->isBoolean())
                    value->parse(argv[pos++]);
                else
                    value->parse();
            }
            else
            {
                if (idx < group_pos.options.size())
                    group_pos.options[idx++].value->parse(key);
            }
        }

        OptionsResult result;
        result.copy(group_key.options);
        result.copy(group_pos.options);

        return result;
    }

    std::string help() const
    {
        return fmt::format(
            "Usage: {}{}{}\n{}{}",
            program,
            group_key.arguments(),
            group_pos.arguments(),
            group_key.help(),
            group_pos.help());
    }

private:
    std::string program;
    detail::OptionGroup group_key;
    detail::OptionGroup group_pos;
};

}  // namespace shell
