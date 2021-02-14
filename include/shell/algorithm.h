#pragma once

#include <cstring>
#include <cwchar>
#include <string>
#include <vector>

#include <shell/locale.h>
#include <shell/ranges.h>

namespace shell
{

namespace detail
{

template<typename String>
std::size_t len(const String& str)
{
    if constexpr (is_specialization_v<String, std::basic_string>)
    {
        return str.size();
    }
    else
    {
        using Char = unqualified_t<String>;
    
        static_assert(is_any_of_v<Char, char, wchar_t>);

        if constexpr (std::is_array_v<String>)
            return sizeof(str) - sizeof(Char);

        if constexpr (std::is_pointer_v<String> && std::is_same_v<Char, char>)
            return std::strlen(str);

        if constexpr (std::is_pointer_v<String> && std::is_same_v<Char, wchar_t>)
            return std::wcslen(str);
    }
}

}  // namespace detail

template<typename String, typename Predicate>
void trimLeftIf(String& str, Predicate pred)
{
    str.erase(
        std::begin(str),
        std::find_if_not(
            std::begin(str),
            std::end(str),
            pred));
}

template<typename String>
void trimLeft(String& str, const std::locale& locale = std::locale())
{
    trimLeftIf(str, IsSpace<range_value_t<String>>(locale));
}

template<typename String, typename Predicate>
String trimLeftCopyIf(const String& str, Predicate pred)
{
    return String(
        std::find_if_not(
            std::begin(str),
            std::end(str),
            pred),
        std::end(str));
}

template<typename String>
String trimLeftCopy(const String& str, const std::locale& locale = std::locale())
{
    return trimLeftCopyIf(str, IsSpace<range_value_t<String>>(locale));
}

template<typename String, typename Predicate>
void trimRightIf(String& str, Predicate pred)
{
    str.erase(
        std::find_if_not(
            std::rbegin(str),
            std::rend(str),
            pred).base(),
        std::end(str));
}

template<typename String>
void trimRight(String& str, const std::locale& locale = std::locale())
{
    trimRightIf(str, IsSpace<range_value_t<String>>(locale));
}

template<typename String, typename Predicate>
String trimRightCopyIf(const String& str, Predicate pred)
{
    return String(
        std::begin(str),
        std::find_if_not(
            std::rbegin(str),
            std::rend(str),
            pred).base());
}

template<typename String>
String trimRightCopy(const String& str, const std::locale& locale = std::locale())
{
    return trimRightCopyIf(str, IsSpace<range_value_t<String>>(locale));
}

template<typename String, typename Predicate>
void trimIf(String& str, Predicate pred)
{
    trimLeftIf(str, pred);
    trimRightIf(str, pred);
}

template<typename String>
void trim(String& str, const std::locale& locale = std::locale())
{
    trimIf(str, IsSpace<range_value_t<String>>(locale));
}

template<typename String, typename Predicate>
String trimCopyIf(const String& str, Predicate pred)
{
    return String(
        std::find_if_not(
            std::begin(str),
            std::end(str),
            pred),
        std::find_if_not(
            std::rbegin(str),
            std::rend(str),
            pred).base());
}

template<typename String>
String trimCopy(const String& seq, const std::locale& locale = std::locale())
{
    return trimCopyIf(seq, IsSpace<range_value_t<String>>(locale));
}

template<typename String>
void toLower(String& str, const std::locale& locale = std::locale())
{
    std::transform(
        std::begin(str),
        std::end(str),
        std::begin(str),
        ToLower<range_value_t<String>>(locale));
}

template<typename String>
String toLowerCopy(const String& str, const std::locale& locale = std::locale())
{
    String res{};
    res.reserve(detail::len(str));

    std::transform(
        std::begin(str),
        std::end(str),
        std::back_inserter(res),
        ToLower<range_value_t<String>>(locale));

    return res;
}

template<typename String>
void toUpper(String& str, const std::locale& locale = std::locale())
{
    std::transform(
        std::begin(str),
        std::end(str),
        std::begin(str),
        ToUpper<range_value_t<String>>(locale));
}

template<typename String>
String toUpperCopy(const String& str, const std::locale& locale = std::locale())
{
    String res{};
    res.reserve(detail::len(str));

    std::transform(
        std::begin(str),
        std::end(str),
        std::back_inserter(res),
        ToUpper<range_value_t<String>>(locale));

    return res;
}

template<typename String, typename From, typename To>
void replaceFirst(String& str, const From& from, const To& to)
{
    std::size_t pos = str.find(from);

    if (pos != String::npos)
        str.replace(pos, detail::len(from), to);
}

template<typename String, typename From, typename To>
String replaceFirstCopy(const String& str, const From& from, const To& to)
{
    String res(str);
    replaceFirst(res, from, to);

    return res;
}

template<typename String, typename From, typename To>
void replaceLast(String& str, const From& from, const To& to)
{
    std::size_t pos = str.rfind(from);

    if (pos != String::npos)
        str.replace(pos, detail::len(from), to);
}

template<typename String, typename From, typename To>
String replaceLastCopy(const String& str, const From& from, const To& to)
{
    String res(str);
    replaceLast(res, from, to);

    return res;
}

template<typename String, typename From, typename To>
void replaceAll(String& str, const From& from, const To& to)
{
    std::size_t pos      = 0;
    std::size_t len_to   = detail::len(to);
    std::size_t len_from = detail::len(from);

    while ((pos = str.find(from, pos)) != String::npos)
    {
        str.replace(pos, len_from, to);
        pos += len_to;
    }
}

template<typename String, typename From, typename To>
String replaceAllCopy(const String& str, const From& from, const To& to)
{
    String res(str);
    replaceAll(res, from, to);

    return res;
}

template<typename OutputIterator, typename String, typename Delimiter>
OutputIterator split(OutputIterator out, const String& str, const Delimiter& del)
{
    std::size_t pos = 0;
    std::size_t end = str.find(del);
    std::size_t len = detail::len(del);

    while (end != String::npos)
    {
        *out = str.substr(pos, end - pos);
         pos = end + len;
         end = str.find(del, pos);
    }
    *out = str.substr(pos, end);

    return out;
}

template<typename String, typename Delimiter>
std::vector<String> split(const String& str, const Delimiter& del)
{
    std::vector<String> res;
    split(std::back_inserter(res), str, del);

    return res;
}

template<typename Range, typename Delimiter>
range_value_t<Range> join(const Range& range, const Delimiter& del)
{
    range_value_t<Range> res{};

    for (auto [pos, str] : enumerate(range))
    {
        if (pos != 0)
            res.append(del);

        res.append(str);
    }
    return res;
}

template<typename String, typename Substring>
bool startsWith(const String& str, const Substring& substr)
{
    return str.compare(0, detail::len(substr), substr) == 0;
}

template<typename String, typename Substring>
bool endsWith(const String& str, const Substring& substr)
{
    std::size_t len_str = detail::len(str);
    std::size_t len_sub = detail::len(substr);

    if (len_sub > len_str)
        return false;

    return str.compare(len_str - len_sub, len_sub, substr) == 0;
}

}  // namespace shell
