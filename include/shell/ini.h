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

    void error(const std::string& expected) const
    {
        std::string got = index < data.size()
            ? std::string(1, data[index])
            : std::string();

        throw ParseError("Expected {} at index {} in line '{}' but got '{}'", expected, index, data, got);
    }

    template<typename Predicate>
    void eat(Predicate pred)
    {
        value.clear();
        if (index < data.size() && pred(data[index]))
        {
            value.push_back(data[index]);
            ++index;
        }
    }

    template<typename Predicate>
    bool eatOne(Predicate pred)
    {
        eat(pred);
        return value.size() == 1;
    }

    template<typename Predicate>
    void consume(Predicate pred)
    {
        value.clear();
        while (index < data.size() && pred(data[index]))
        {
            value.push_back(data[index]);
            ++index;
        }
    }

    template<typename Predicate>
    bool consumeOne(Predicate pred)
    {
        consume(pred);
        return value.size() == 1;
    }

    template<typename Predicate>
    bool consumeSome(Predicate pred)
    {
        consume(pred);
        return value.size() != 0;
    }

    std::size_t index = 0;
    std::string value;

private:
    const std::string& data;
};

class Token
{
public:
    enum class Kind { Blank, Comment, Section, Value };

    Token(Kind kind)
        : kind(kind) {}

    virtual void parse(const std::string& line) = 0;
    virtual std::string string() const = 0;

    Kind kind;
};

class BlankToken final : public Token
{
public:
    BlankToken()
        : Token(Kind::Blank) {}

    void parse(const std::string& line) final {}

    std::string string() const final
    {
        return std::string();
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

        if (!parser.eatOne([](char ch) {
            return ch == '#'
                || ch == ';';
            }))
            parser.error("'#' or ';'");

        token = parser.value;

        parser.consume(IsSpace<char>());
        parser.consume(Tautology());

        comment = parser.value;
    }

    std::string string() const final
    {
        return fmt::format("{} {}", token, comment);
    }

    std::string token;
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

        if (!parser.eatOne([](char ch) {
                return ch == '['; 
            }))
            parser.error("'['");

        if (!parser.consumeSome([](char ch) {
            return IsAlnum<char>()(ch)
                || IsSpace<char>()(ch)
                || ch == '-'
                || ch == '_'
                || ch == ':';
            }))
            parser.error("section char");

        section = parser.value;

        if (!parser.eatOne([](char ch) {
                return ch == ']';
            }))
            parser.error("']'");

        if (parser.consumeSome(Tautology()))
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

        if (!parser.consumeSome([](char ch) {
                return std::isalnum(ch)
                    || ch == '_';
            }))
            parser.error("key char");

        key = parser.value;

        parser.consume(IsSpace<char>());

        if (!parser.eatOne([](char ch) {
                return ch == '=';
            }))
            parser.error("'='");

        parser.consume(IsSpace<char>());
        parser.consume(Tautology());

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

        switch (line.front())
        {
        case '#': return std::make_shared<detail::CommentToken>();
        case ';': return std::make_shared<detail::CommentToken>();
        case '[': return std::make_shared<detail::SectionToken>();
        }
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
