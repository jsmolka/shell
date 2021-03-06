#pragma once

#include <locale>

namespace shell
{

namespace detail
{

template<typename Char, typename std::ctype_base::mask kMask>
class IsClassifiedAs
{
public:
    IsClassifiedAs(const std::locale& locale = std::locale())
        : _locale(locale) {}

    bool operator()(Char ch) const
    {
        return std::use_facet<std::ctype<Char>>(_locale).is(kMask, ch);
    }

private:
    const std::locale& _locale;
};

}  // namespace detail

template<typename Char> using IsAlnum  = detail::IsClassifiedAs<Char, std::ctype_base::alnum>;
template<typename Char> using IsAlpha  = detail::IsClassifiedAs<Char, std::ctype_base::alpha>;
template<typename Char> using IsBlank  = detail::IsClassifiedAs<Char, std::ctype_base::blank>;
template<typename Char> using IsCntrl  = detail::IsClassifiedAs<Char, std::ctype_base::cntrl>;
template<typename Char> using IsDigit  = detail::IsClassifiedAs<Char, std::ctype_base::digit>;
template<typename Char> using IsGraph  = detail::IsClassifiedAs<Char, std::ctype_base::graph>;
template<typename Char> using IsLower  = detail::IsClassifiedAs<Char, std::ctype_base::lower>;
template<typename Char> using IsPrint  = detail::IsClassifiedAs<Char, std::ctype_base::print>;
template<typename Char> using IsPunct  = detail::IsClassifiedAs<Char, std::ctype_base::punct>;
template<typename Char> using IsSpace  = detail::IsClassifiedAs<Char, std::ctype_base::space>;
template<typename Char> using IsUpper  = detail::IsClassifiedAs<Char, std::ctype_base::upper>;
template<typename Char> using IsLower  = detail::IsClassifiedAs<Char, std::ctype_base::lower>;
template<typename Char> using IsXDigit = detail::IsClassifiedAs<Char, std::ctype_base::xdigit>;

template<typename Char>
class ToLower
{
public:
    ToLower(const std::locale& locale = std::locale())
        : _locale(locale) {}

    Char operator()(Char ch) const
    {
        return std::tolower(ch, _locale);
    }

private:
    const std::locale& _locale;
};

template<typename Char>
class ToUpper
{
public:
    ToUpper(const std::locale& locale = std::locale())
        : _locale(locale) {}

    Char operator()(Char ch) const
    {
        return std::toupper(ch, _locale);
    }

private:
    const std::locale& _locale;
};

}  // namespace shell
