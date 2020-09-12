#pragma once

#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <eggcpt/algorithm.h>
#include <eggcpt/errors.h>

namespace eggcpt
{

class OptionParseError : public FormattedError
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
        : m_optional(optional)
        , m_positional(false) {}

    virtual ~Value() = default;

    Pointer optional()
    {
        m_optional = true;
        return shared_from_this();
    }

    bool isOptional() const
    {
        return m_optional;
    }

    Pointer positional()
    {
        m_positional = true;
        return shared_from_this();
    }

    bool isPositional() const
    {
        return m_positional;
    }

    virtual void parse() = 0;
    virtual void parse(const std::string& data) = 0;
    virtual bool hasValue() const = 0;
    virtual bool hasDefaultValue() const = 0;
    virtual std::string getValue() const = 0;
    virtual std::string getDefaultValue() const = 0;

private:
    bool m_optional;
    bool m_positional;
};

template<typename T>
class OptionValue : public Value
{
public:
    OptionValue()
        : Value(false)
        , value(std::nullopt)
        , default_value(std::nullopt) {}

    explicit OptionValue(const T& value)
        : Value(true)
        , value(value)
        , default_value(value) {}

    void parse() override
    {
        if constexpr (std::is_same_v<T, bool>)
            value = true;
        else
            throw OptionParseError("Missing value for non-bool option");
    }

    void parse(const std::string& data) override
    {
        if constexpr (std::is_same_v<T, bool>)
        {
                 if (data == "1" || data == "true" ) value = true;
            else if (data == "0" || data == "false") value = false;
            else throw OptionParseError("Invalid bool data: {}", data);
        }
        else
        {
            T result;

            std::stringstream stream;
            stream << data;
            stream >> result;

            throwIf<OptionParseError>(!stream, "Invalid data: {}", data);

            this->value = result;
        }
    }

    bool hasValue() const override
    {
        return value.has_value();
    }

    bool hasDefaultValue() const override
    {
        return default_value.has_value();
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

struct Option
{
    std::vector<std::string> keys;
    Value::Pointer value;
    std::string desc;
};

class OptionVector : public std::vector<Option>
{
public:
    Value::Pointer find(const std::string& key) const
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
            const auto key = join(keys, kDelimiter);
            const auto def = value->hasDefaultValue()
                ? fmt::format(" (default: {})", value->getDefaultValue())
                : std::string();

            result.append(fmt::format("{:<{}}{}{}\n", key, padding, desc, def));
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
            std::size_t size = 0;
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
    T get(const std::string& key) const
    {
        return *std::static_pointer_cast<
            detail::OptionValue<T>>(options.find(key))->value;
    }

private:
    void copyAndValidate(detail::OptionVector& other)
    {
        for (const auto& [keys, value, desc] : other)
        {
            if (value->hasValue())
                options.push_back({ keys, value, desc });
            else
                throwIf<OptionParseError>(!value->isOptional(), "Missing option: {}", join(keys, ", "));
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

    void add(const std::vector<std::string>& keys, const std::string& desc, detail::Value::Pointer value)
    {
        auto& options = value->isPositional()
            ? positional.options
            : keyword.options;

        options.push_back({ keys, value, desc });
    }

    OptionsResult parse(int argc, const char* argv[])
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
        result.copyAndValidate(keyword.options);
        result.copyAndValidate(positional.options);

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
