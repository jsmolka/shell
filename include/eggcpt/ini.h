#pragma once

#include <eggcpt/errors.h>
#include <eggcpt/filesystem.h>
#include <eggcpt/fmt.h>

namespace eggcpt::ini
{

class ParseError : public FormattedError
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

        throwIf<ParseError>(
            !consumer.eatOne([](char ch) {
                return ch == '#'
                    || ch == ';';
            }),
            "Expected # or ; at position {}: {}", consumer.pos, line);
        token = consumer.value;

        consumer.consume(std::isspace);
        consumer.consume([](char ch) {
            return true;
        });
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

        throwIf<ParseError>(
            !consumer.eatOne([](char ch) {
                return ch == '[';
            }),
            "Expected [ at position {}: {}", consumer.pos, line);

        throwIf<ParseError>(
            !consumer.consumeSome([](char ch) {
                return std::isalnum(ch)
                    || ch == '_';
            }),
            "Expected section char at position {}: {}", consumer.pos, line);
        section = consumer.value;

        throwIf<ParseError>(
            !consumer.eatOne([](char ch) {
                return ch == ']';
            }),
            "Expected ] at position {}: {}", consumer.pos, line);

        throwIf<ParseError>(
            consumer.consumeSome([](char ch) {
                return true;
            }),
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

        throwIf<ParseError>(
            !consumer.consumeSome([](char ch) {
                return std::isalnum(ch)
                    || ch == '_';
            }),
            "Expected key char at position {}: {}", consumer.pos, line);
        key = consumer.value;

        consumer.consume(std::isspace);

        throwIf<ParseError>(
            !consumer.eatOne([](char ch) {
                return ch == '=';
            }),
            "Expected = at position {}: {}", consumer.pos, line);

        consumer.consume(std::isspace);
        consumer.consume([](char ch) {
            return true;
        });
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

}  // namespace eggcpt::toml
