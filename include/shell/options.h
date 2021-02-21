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

    OptionSpec() = default;

    OptionSpec(
        const std::string& opts,
        const std::string& desc = std::string(),
        const std::string& name = std::string())
        : opts(split(opts, ",")), desc(desc), name(name) {}

    std::string argument() const
    {
        std::string_view format = _positional
            ? "<{}>" : name.size() ? "{} <{}>" : "{}";

        return fmt::format(format, opts.front(), name);
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

    Pointer optional()
    {
        _optional = true;
        return shared_from_this();
    }

    bool isPositional() const
    {
        return _positional;
    }

    Pointer positional()
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
class OptionValue : public Value
{
public:
    OptionValue() = default;

    OptionValue(const T& value)
    {
        this->value = value;
        this->_default = value;
        this->_optional = true;
    }

    void parse()
    {
        if constexpr (std::is_same_v<T, bool>)
            value = true;
        else
            throw ParseError("Expected data but got none");
    }

    void parse(const std::string& data)
    {
        if (!(value = shell::parse<T>(data)))
            throw ParseError("Cannot parse '{}'", data);
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

private:
    std::optional<T> _default;
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

class OptionGroup : public OptionVector
{
public:
    OptionGroup(std::string_view name)
        : _name(name) {}

    std::string arguments() const
    {
        std::string args;
        
        for (const auto& [spec, value] : *this)
        {
            std::string_view format = value->isOptional() ? " [{}]" : " {}";
            args.append(fmt::format(format, spec.argument()));
        }
        return args;
    }

    std::string help() const
    {
        if (empty())
            return std::string();

        std::size_t padding = 0;
        std::vector<std::string> keys;
        keys.reserve(size());
        
        for (const auto& [spec, value] : *this)
        {
            keys.push_back(join(spec.opts, ","));
            padding = std::max(padding, keys.back().size());
        }

        std::string help = fmt::format("\n{} arguments:\n", _name);

        for (const auto [option, key] : zip(*this, keys))
        {
            help.append(fmt::format(
                "{:<{}}{}{}\n",
                key,
                padding + 2,
                option.spec.desc,
                option.value->help()));
        }
        return help;
    }

private:
    std::string_view _name;
};

}  // namespace detail

class OptionsResult
{
public:
    friend class Options;

    bool has(const std::string& key) const
    {
        return _options.has(key);
    }

    template<typename T>
    std::optional<T> find(const std::string& key) const
    {
        if (const auto value = _options.find(key))
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
                _options.push_back({ spec, value });
            else if (!value->isOptional())
                throw ParseError("Expected value for option '{}' but got none", spec.opts.front());
        }
    }

    detail::OptionVector _options;
};

class Options
{
public:
    Options(const std::string& program)
        : _program(program)
        , _keyword("Keyword")
        , _positional("Positional") {}

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

    void add(OptionSpec spec, detail::Value::Pointer value)
    {
        SHELL_ASSERT(spec.opts.size());

        spec._positional = value->isPositional();

        auto& options = spec._positional
            ? _positional
            : _keyword;

        options.push_back({ spec, value });
    }

    OptionsResult parse(int argc, const char* const* argv)
    {
        int idx = 1;
        int pos = 0;

        while (idx < argc)
        {
            auto arg = std::string(argv[idx++]);
            auto kvp = splitFirst(arg, "=");

            if (auto value = _keyword.find(kvp.front()))
            {
                if (kvp.size() == 2)
                    value->parse(kvp.back());
                else if (idx < argc && !value->isBoolean() && !_keyword.has(argv[idx]))
                    value->parse(argv[idx++]);
                else
                    value->parse();
            }
            else
            {
                if (pos < _positional.size())
                    _positional[pos++].value->parse(arg);
            }
        }

        OptionsResult result;
        result.copy(_keyword);
        result.copy(_positional);

        return result;
    }

    std::string help() const
    {
        return fmt::format(
            "Usage: {}{}{}\n{}{}",
            _program,
            _keyword.arguments(),
            _positional.arguments(),
            _keyword.help(),
            _positional.help());
    }

private:
    std::string _program;
    detail::OptionGroup _keyword;
    detail::OptionGroup _positional;
};

}  // namespace shell
