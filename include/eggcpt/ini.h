#pragma once

#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include <eggcpt/algorithm.h>
#include <eggcpt/errors.h>
#include <eggcpt/filesystem.h>
#include <eggcpt/functional.h>
#include <eggcpt/fmt.h>
#include <eggcpt/parse.h>

namespace eggcpt
{

class IniParseError : public FormattedError
{
public:
    using FormattedError::FormattedError;
};

namespace detail
{

class Consumer
{
public:
    Consumer(const std::string& data)
        : data(data)
    {
        value.reserve(data.size());
    }

    template<typename Predicate>
    void eat(Predicate pred)
    {
        value.clear();
        if (pos < data.size() && pred(data[pos]))
        {
            value.push_back(data[pos]);
            ++pos;
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
        while (pos < data.size() && pred(data[pos]))
        {
            value.push_back(data[pos]);
            ++pos;
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

    std::size_t pos = 0;
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

class BlankToken : public Token
{
public:
    BlankToken()
        : Token(Kind::Blank) {}

    void parse(const std::string& line) override {}

    std::string string() const override
    {
        return std::string();
    }
};

class CommentToken : public Token
{
public:
    CommentToken()
        : Token(Kind::Comment) {}

    void parse(const std::string& line) override
    {
        Consumer consumer(line);

        throwIf<IniParseError>(
            !consumer.eatOne([](char ch) {
                return ch == '#'
                    || ch == ';';
            }),
            "Expected # or ; at position {}: {}", consumer.pos, line);
        token = consumer.value;

        consumer.consume(IsSpace<char>());
        consumer.consume(Tautology());
        comment = consumer.value;
    }

    std::string string() const override
    {
        return fmt::format("{} {}", token, comment);
    }

    std::string token;
    std::string comment;
};

class SectionToken : public Token
{
public:
    SectionToken()
        : Token(Kind::Section) {}

    void parse(const std::string& line) override
    {
        Consumer consumer(line);

        throwIf<IniParseError>(
            !consumer.eatOne([](char ch) {
                return ch == '[';
            }),
            "Expected [ at position {}: {}", consumer.pos, line);

        throwIf<IniParseError>(
            !consumer.consumeSome([](char ch) {
                return std::isalnum(ch)
                    || ch == '_';
            }),
            "Expected section char at position {}: {}", consumer.pos, line);
        section = consumer.value;

        throwIf<IniParseError>(
            !consumer.eatOne([](char ch) {
                return ch == ']';
            }),
            "Expected ] at position {}: {}", consumer.pos, line);

        throwIf<IniParseError>(
            consumer.consumeSome(Tautology()),
            "Expected no chars at position {}: {}", consumer.pos, line);
    }

    std::string string() const override
    {
        return fmt::format("[{}]", section);
    }

    std::string section;
};

class ValueToken : public Token
{
public:
    ValueToken()
        : Token(Kind::Value) {}

    void parse(const std::string& line) override
    {
        Consumer consumer(line);

        throwIf<IniParseError>(
            !consumer.consumeSome([](char ch) {
                return std::isalnum(ch)
                    || ch == '_';
            }),
            "Expected key char at position {}: {}", consumer.pos, line);
        key = consumer.value;

        consumer.consume(IsSpace<char>());

        throwIf<IniParseError>(
            !consumer.eatOne([](char ch) {
                return ch == '=';
            }),
            "Expected = at position {}: {}", consumer.pos, line);

        consumer.consume(IsSpace<char>());
        consumer.consume(Tautology());
        value = consumer.value;
    }

    std::string string() const override
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
    bool load(const filesystem::path& file)
    {
        auto stream = std::ifstream(file);
        if (!stream.is_open())
            return false;

        std::string line;
        while (std::getline(stream, line))
        {
            trim(line);

            Token token = makeToken(line);
            token->parse(line);

            tokens.push_back(token);
        }
        return true;
    }

    bool save(const filesystem::path& file) const
    {
        std::vector<std::string> lines;
        lines.reserve(tokens.size());

        for (const auto& token : tokens)
            lines.push_back(token->string());

        return filesystem::write(file, join(lines, "\n"));
    }

    template<typename T>
    std::optional<T> find(const std::string& section, const std::string& key) const
    {
        if (const auto token = findToken(section, key))
            return parse<T>(token->value);

        return std::nullopt;
    }

    template<typename T>
    T findOr(const std::string& section, const std::string& key, const T& fallback) const
    {
        return find<T>(section, key).value_or(fallback);
    }

    void set(const std::string& section, const std::string& key, const std::string& value)
    {
        if (const auto token = findToken(section, key))
            token->value = value;
    }

private:
    using Token = std::shared_ptr<detail::Token>;
    using ValueToken = std::shared_ptr<detail::ValueToken>;

    static Token makeToken(const std::string& line)
    {
        if (line.empty()) return std::make_shared<detail::BlankToken>();
        if (line.front() == '#') return std::make_shared<detail::CommentToken>();
        if (line.front() == '[') return std::make_shared<detail::SectionToken>();
        return std::make_shared<detail::ValueToken>();
    }

    ValueToken findToken(const std::string& section, const std::string& key) const
    {
        std::string active;
        for (const auto& token : tokens)
        {
            switch (token->kind)
            {
            case detail::Token::Kind::Section:
                active = std::static_pointer_cast<detail::SectionToken>(token)->section;
                break;

            case detail::Token::Kind::Value:
                if (section == active 
                        && std::static_pointer_cast<detail::ValueToken>(token)->key == key)
                    return std::static_pointer_cast<detail::ValueToken>(token);
                break;
            }
        }
        return nullptr;
    }

    std::vector<Token> tokens;
};

}  // namespace eggcpt
