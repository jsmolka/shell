#pragma once

#include <locale>

#include <eggcpt/macros.h>

namespace eggcpt
{

template<typename Char>
class IsAlnum
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsAlnum(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        return std::isalnum(ch, locale);
    }

private:
    const std::locale& locale;
};

template<typename Char>
class IsAlpha
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsAlpha(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        return std::isalpha(ch, locale);
    }

private:
    const std::locale& locale;
};

template<typename Char>
class IsBlank
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsBlank(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        #if EGGCPT_OS_DARWIN && EGGCPT_CC_CLANG
        // Workaround for macOS clang using the wrong isblank
        return std::use_facet<std::ctype<Char>>(locale).is(std::ctype_base::alpha, ch);        
        #else
        return std::isblank(ch, locale);
        #endif
    }

private:
    const std::locale& locale;
};

template<typename Char>
class IsCntrl
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsCntrl(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        return std::iscntrl(ch, locale);
    }

private:
    const std::locale& locale;
};

template<typename Char>
class IsDigit
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsDigit(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        return std::isdigit(ch, locale);
    }

private:
    const std::locale& locale;
};

template<typename Char>
class IsGraph
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsGraph(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        return std::isgraph(ch, locale);
    }

private:
    const std::locale& locale;
};

template<typename Char>
class IsLower
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsLower(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        return std::islower(ch, locale);
    }

private:
    const std::locale& locale;
};

template<typename Char>
class IsPrint
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsPrint(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        return std::isprint(ch, locale);
    }

private:
    const std::locale& locale;
};

template<typename Char>
class IsPunct
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsPunct(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        return std::ispunct(ch, locale);
    }

private:
    const std::locale& locale;
};

template<typename Char>
class IsSpace
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsSpace(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        return std::isspace(ch, locale);
    }

private:
    const std::locale& locale;
};

template<typename Char>
class IsUpper
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsUpper(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        return std::isupper(ch, locale);
    }

private:
    const std::locale& locale;
};

template<typename Char>
class IsXDigit
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsXDigit(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        return std::isxdigit(ch, locale);
    }

private:
    const std::locale& locale;
};

template<typename Char>
class ToLower
{
public:
    using argument_type = Char;
    using result_type = Char;

    explicit ToLower(const std::locale& locale = std::locale())
        : locale(locale) {}

    Char operator()(const Char& ch) const
    {
        return std::tolower(ch, locale);
    }

private:
    const std::locale& locale;
};

template<typename Char>
class ToUpper
{
public:
    using argument_type = Char;
    using result_type = Char;

    explicit ToUpper(const std::locale& locale = std::locale())
        : locale(locale) {}

    Char operator()(const Char& ch) const
    {
        return std::toupper(ch, locale);
    }

private:
    const std::locale& locale;
};

}  // namespace eggcpt
