#pragma once

#include <locale>

namespace eggcpt
{

namespace detail
{

template<typename Char, std::size_t Type>
class IsCharType
{
public:
    using argument_type = Char;
    using result_type = bool;

    explicit IsCharType(const std::locale& locale = std::locale())
        : locale(locale) {}

    bool operator()(const Char& ch) const
    {
        return std::use_facet<std::ctype<Char>>(locale).is(Type, ch);
    }

private:
    const std::locale& locale;
};

}  // namespace detail

template<typename Char> using IsAlnum  = detail::IsCharType<Char, std::ctype_base::alnum>;
template<typename Char> using IsAlpha  = detail::IsCharType<Char, std::ctype_base::alpha>;
template<typename Char> using IsBlank  = detail::IsCharType<Char, std::ctype_base::blank>;
template<typename Char> using IsCntrl  = detail::IsCharType<Char, std::ctype_base::cntrl>;
template<typename Char> using IsDigit  = detail::IsCharType<Char, std::ctype_base::digit>;
template<typename Char> using IsGraph  = detail::IsCharType<Char, std::ctype_base::graph>;
template<typename Char> using IsLower  = detail::IsCharType<Char, std::ctype_base::lower>;
template<typename Char> using IsPrint  = detail::IsCharType<Char, std::ctype_base::print>;
template<typename Char> using IsPunct  = detail::IsCharType<Char, std::ctype_base::punct>;
template<typename Char> using IsSpace  = detail::IsCharType<Char, std::ctype_base::space>;
template<typename Char> using IsUpper  = detail::IsCharType<Char, std::ctype_base::upper>;
template<typename Char> using IsLower  = detail::IsCharType<Char, std::ctype_base::lower>;
template<typename Char> using IsXDigit = detail::IsCharType<Char, std::ctype_base::xdigit>;

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
