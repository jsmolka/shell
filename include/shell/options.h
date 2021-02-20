#pragma once

#include <memory>
#include <vector>

#include <shell/algorithm.h>
#include <shell/errors.h>
#include <shell/macros.h>
#include <shell/parse.h>

namespace shell
{

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
        SHELL_ASSERT(opts.size());
    }

    std::string options() const
    {
        return join(opts, ", ");
    }

    std::string argument() const
    {
        std::string_view pattern = _positional
            ? "<{}>" : name.size() ? "{} <{}>" : "{}";

        return fmt::format(pattern, opts.front(), name);
    }

    std::vector<std::string> opts;
    std::string desc;
    std::string name;

private:
    bool _positional = false;
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
        return _optional;
    }

    virtual Pointer optional()
    {
        _optional = true;
        return shared_from_this();
    }

    bool isPositional() const
    {
        return _positional;
    }

    virtual Pointer positional()
    {
        _positional = true;
        return shared_from_this();
    }
       
    virtual void parse() = 0;
    virtual void parse(const std::string& data) = 0;
    virtual bool isEmpty() const = 0;
    virtual bool isBoolean() const = 0;
    virtual std::string help() const = 0;

protected:
    bool _optional = false;
    bool _positional = false;
};

template<typename T>
class BasicValue : public Value
{
public:
    void parse(const std::string& data)
    {
        if (!(value = shell::parse<T>(data)))
            throw ParseError("Expected data but got '{}'", data);
    }

    bool isEmpty() const
    {
        return !value;
    }

    bool isBoolean() const
    {
        return std::is_same_v<T, bool>;
    }

    std::string help() const
    {
        if (_default)  return fmt::format(" (default: {})", *_default);
        if (_optional) return fmt::format(" (optional)");

        return std::string();
    }

    std::optional<T> value;

protected:
    std::optional<T> _default;
};

template<typename T>
class OptionValue : public BasicValue<T>
{
public:
    OptionValue() = default;

    OptionValue(const T& value)
    {
        this->value = value;
        this->_default = value;
        this->_optional = true;
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
        this->_default = false;
        this->_optional = true;
    }

    Pointer positional() final
    {
        SHELL_ASSERT(false);
        return shared_from_this();
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
    OptionGroup(std::string_view name)
        : _name(name) {}

    std::string arguments() const
    {
        std::string arguments;
        for (const auto& [spec, value] : options)
        {
            std::string_view pattern = value->isOptional() ? " [{}]" : " {}";
            arguments.append(fmt::format(pattern, spec.argument()));
        }
        return arguments;
    }

    std::string help() const
    {
        if (options.empty())
            return std::string();

        std::size_t offset = padding();
        std::string result = fmt::format("\n{} arguments:\n", _name);

        for (const auto& [spec, value] : options)
        {
            result.append(fmt::format(
                "{:<{}}{}{}\n",
                spec.options(),
                offset,
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

    std::string_view _name;
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
            if (!value->isEmpty())
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
        spec._positional = value->isPositional();

        auto& options = value->isPositional()
            ? group_pos.options
            : group_key.options;

        options.push_back({ spec, value });
    }

    OptionsResult parse(int argc, const char* const* argv)
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
