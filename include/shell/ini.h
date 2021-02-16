#pragma once

#include <memory>
#include <vector>

#include <shell/algorithm.h>
#include <shell/errors.h>
#include <shell/filesystem.h>
#include <shell/functional.h>
#include <shell/parse.h>
#include <shell/ranges.h>

namespace shell
{

namespace detail
{

class Parser
{
public:
    Parser(const std::string& data)
        : data(data)
    {
        value.reserve(data.size());
    }

    template<typename Predicate>
    std::size_t one(Predicate pred)
    {
        value.clear();
        if (index < data.size() && pred(data[index]))
        {
            value.push_back(data[index]);
            ++index;
        }
        return value.size();
    }

    template<typename Predicate>
    std::size_t all(Predicate pred)
    {
        value.clear();
        while (index < data.size() && pred(data[index]))
        {
            value.push_back(data[index]);
            ++index;
        }
        return value.size();
    }

    void error(const std::string& expected) const
    {
        std::string got = index < data.size()
            ? std::string(1, data[index])
            : std::string();

        throw ParseError(
            "Expected {} at index {} in '{}' but got '{}'",
            expected, index, data, got);
    }

    std::string value;
    std::size_t index = 0;

private:
    const std::string& data;
};

class Token
{
public:
    enum class Kind { Comment, Section, Value };

    Token(Kind kind)
        : kind(kind) {}

    virtual void parse(const std::string& line) = 0;
    virtual std::string string() const = 0;

    const Kind kind;

protected:
    template<char Char>
    static bool isChar(char ch)
    {
        return ch == Char;
    }

    static bool isIdentifier(char ch)
    {
        return IsAlnum<char>()(ch) || ch == '_';
    }
};

class CommentToken final : public Token
{
public:
    CommentToken()
        : Token(Kind::Comment) {}

    void parse(const std::string& line) final
    {
        Parser parser(line);

        if (!parser.one(isChar<'#'>))
            parser.error("'#'");

        parser.all(IsSpace<char>());
        parser.all(Tautology());

        comment = parser.value;
    }

    std::string string() const final
    {
        return fmt::format("# {}", comment);
    }

    std::string comment;
};

class SectionToken final : public Token
{
public:
    SectionToken()
        : Token(Kind::Section) {}

    SectionToken(const std::string& section)
        : Token(Kind::Section), section(section) {}

    void parse(const std::string& line) final
    {
        Parser parser(line);

        if (!parser.one(isChar<'['>))
            parser.error("'['");

        if (!parser.all(isIdentifier))
            parser.error("identifier char");

        section = parser.value;

        if (!parser.one(isChar<']'>))
            parser.error("']'");

        if (parser.all(Tautology()))
            parser.error("no char");
    }

    std::string string() const final
    {
        return fmt::format("[{}]", section);
    }

    std::string section;
};

class ValueToken final : public Token
{
public:
    ValueToken()
        : Token(Kind::Value) {}

    ValueToken(const std::string& key, const std::string& value)
        : Token(Kind::Value), key(key), value(value) {}

    void parse(const std::string& line) final
    {
        Parser parser(line);

        if (!parser.all(isIdentifier))
            parser.error("identifier char");

        key = parser.value;

        parser.all(IsSpace<char>());

        if (!parser.one(isChar<'='>))
            parser.error("'='");

        parser.all(IsSpace<char>());
        parser.all(Tautology());

        value = parser.value;
    }

    std::string string() const final
    {
        return fmt::format("{} = {}", key, value);
    }

    std::string key;
    std::string value;
};

}  // namespace detail

class Ini
{
public:
    void parse(const std::string& data)
    {
        tokens.clear();

        for (std::string& line : split(data, "\n"))
        {
            trim(line);

            if (Token token = tokenize(line))
            {
                token->parse(line);

                tokens.push_back(token);
            }
        }
    }

    filesystem::Status load(const filesystem::path& file)
    {
        auto [status, data] = filesystem::read<std::string>(file);

        if (status == filesystem::Status::Ok)
            parse(data);

        return status;
    }

    filesystem::Status save(const filesystem::path& file) const
    {
        std::vector<std::string> lines;

        for (auto [index, token] : enumerate(tokens))
        {
            if (index && token->kind == detail::Token::Kind::Section)
                lines.push_back(std::string());

            lines.push_back(token->string());
        }
        lines.push_back(std::string());

        return filesystem::write(file, join(lines, "\n"));
    }

    template<typename T>
    std::optional<T> find(const std::string& section, const std::string& key) const
    {
        if (const auto token = findToken(section, key))
            return shell::parse<T>(token->value);

        return std::nullopt;
    }

    template<typename T>
    T findOr(const std::string& section, const std::string& key, const T& fallback) const
    {
        return find<T>(section, key).value_or(fallback);
    }

    void set(const std::string& section, const std::string& key, const std::string& value)
    {
        findOrCreateToken(section, key)->value = value;
    }

private:
    using Token      = std::shared_ptr<detail::Token>;
    using ValueToken = std::shared_ptr<detail::ValueToken>;

    static Token tokenize(const std::string& line)
    {
        if (line.empty())
            return nullptr;

        if (line.front() == '#')
            return std::make_shared<detail::CommentToken>();

        if (line.front() == '[')
            return std::make_shared<detail::SectionToken>();

        return std::make_shared<detail::ValueToken>();
    }

    ValueToken findToken(const std::string& section, const std::string& key) const
    {
        std::string active;

        for (const auto& token : tokens)
        {
            if (token->kind == detail::Token::Kind::Section)
            {
                active = std::static_pointer_cast<detail::SectionToken>(token)->section;
            }

            if (token->kind == detail::Token::Kind::Value)
            {
                ValueToken value = std::static_pointer_cast<detail::ValueToken>(token);

                if (active == section && value->key == key)
                    return value;
            }
        }
        return nullptr;
    }

    ValueToken findOrCreateToken(const std::string& section, const std::string& key)
    {
        auto insert = [&](std::vector<Token>::const_iterator iter) -> ValueToken
        {
            ValueToken value = std::make_shared<detail::ValueToken>(key, std::string());
            tokens.insert(iter, value);

            return value;
        };

        std::string active;

        for (auto iter = tokens.begin(); iter != tokens.end(); ++iter)
        {
            const auto& token = *iter;

            if (token->kind == detail::Token::Kind::Section)
            {
                if (active == section)
                    return insert(iter);

                active = std::static_pointer_cast<detail::SectionToken>(token)->section;
            }

            if (token->kind == detail::Token::Kind::Value)
            {
                ValueToken value = std::static_pointer_cast<detail::ValueToken>(token);

                if (active == section && value->key == key)
                    return value;
            }
        }

        if (active != section)
            tokens.push_back(std::make_shared<detail::SectionToken>(section));
        
        return insert(tokens.end());
    }

    std::vector<Token> tokens;
};

}  // namespace shell
