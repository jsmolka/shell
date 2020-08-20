#pragma once

#include <algorithm>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

namespace eggcpt
{

namespace detail
{

inline auto predicateIsSpace(const std::locale& locale)
{
    return [&](auto ch) {
        return std::isspace(ch, locale);
    };
}

}  // namespace detail

template<typename Sequence, typename Predicate>
void trimLeftIf(Sequence& seq, Predicate pred)
{
    seq.erase(
        std::begin(seq),
        std::find_if_not(
            std::begin(seq),
            std::end(seq),
            pred));
}

template<typename Sequence>
void trimLeft(Sequence& seq, const std::locale& locale = std::locale())
{
    trimLeftIf(seq, detail::predicateIsSpace(locale));
}

template<typename Sequence, typename Predicate>
Sequence trimLeftCopyIf(const Sequence& seq, Predicate pred)
{
    return Sequence(
        std::find_if_not(
            std::begin(seq),
            std::end(seq), 
            pred),
        std::end(seq));
}

template<typename OutputIterator, typename Sequence, typename Predicate>
OutputIterator trimLeftCopyIf(OutputIterator output, const Sequence& seq, Predicate pred)
{
    return std::copy(
        std::find_if_not(
            std::begin(seq),
            std::end(seq),
            pred),
        std::end(seq),
        output);
}

template<typename Sequence>
Sequence trimLeftCopy(const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimLeftCopyIf(seq, detail::predicateIsSpace(locale));
}

template<typename OutputIterator, typename Sequence>
OutputIterator trimLeftCopy(OutputIterator output, const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimLeftCopyIf(output, seq, detail::predicateIsSpace(locale));
}

template<typename Sequence, typename Predicate>
void trimRightIf(Sequence& seq, Predicate pred)
{
    seq.erase(
        std::find_if_not(
            std::rbegin(seq),
            std::rend(seq),
            pred).base(),
        std::end(seq));
}

template<typename Sequence>
void trimRight(Sequence& seq, const std::locale& locale = std::locale())
{
    trimRightIf(seq, detail::predicateIsSpace(locale));
}

template<typename Sequence, typename Predicate>
Sequence trimRightCopyIf(const Sequence& seq, Predicate pred)
{
    return Sequence(
        std::begin(seq),
        std::find_if_not(
            std::rbegin(seq),
            std::rend(seq),
            pred).base());
}

template<typename OutputIterator, typename Sequence, typename Predicate>
OutputIterator trimRightCopyIf(OutputIterator output, const Sequence& seq, Predicate pred)
{
    return std::copy(
        std::begin(seq),
        std::find_if_not(
            std::rbegin(seq),
            std::rend(seq),
            pred).base(),
        output);
}

template<typename Sequence>
Sequence trimRightCopy(const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimRightCopyIf(seq, detail::predicateIsSpace(locale));
}

template<typename OutputIterator, typename Sequence>
OutputIterator trimRightCopy(OutputIterator output, const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimRightCopyIf(output, seq, detail::predicateIsSpace(locale));
}

template<typename Sequence, typename Predicate>
void trimIf(Sequence& seq, Predicate pred)
{
    trimLeftIf(seq, pred);
    trimRightIf(seq, pred);
}

template<typename Sequence>
void trim(Sequence& seq, const std::locale& locale = std::locale())
{
    trimIf(seq, detail::predicateIsSpace(locale));
}

template<typename Sequence, typename Predicate>
Sequence trimCopyIf(const Sequence& seq, Predicate pred)
{
    return Sequence(
        std::find_if_not(
            std::begin(seq),
            std::end(seq), 
            pred),
        std::find_if_not(
            std::rbegin(seq),
            std::rend(seq),
            pred).base());
}

template<typename OutputIterator, typename Sequence, typename Predicate>
OutputIterator trimCopyIf(OutputIterator output, const Sequence& seq, Predicate pred)
{
    return std::copy(
        std::find_if_not(
            std::begin(seq),
            std::end(seq), 
            pred),
        std::find_if_not(
            std::rbegin(seq),
            std::rend(seq),
            pred).base(),
        output);
}

template<typename Sequence>
Sequence trimCopy(const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimCopyIf(seq, detail::predicateIsSpace(locale));
}

template<typename OutputIterator, typename Sequence>
OutputIterator trimCopy(OutputIterator output, const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimCopyIf(output, seq, detail::predicateIsSpace(locale));
}

template<typename Sequence>
void replaceLeft(Sequence& seq, const Sequence& from, const Sequence& to)
{
    auto pos = seq.find(from);
    if (pos != Sequence::npos)
        seq.replace(pos, from.length(), to);
}

template<typename Sequence>
Sequence replaceLeftCopy(const Sequence& seq, const Sequence& from, const Sequence& to)
{
    Sequence copy(seq);
    replaceLeft(copy, from, to);
    
    return copy;
}

template<typename Sequence>
void replaceRight(Sequence& seq, const Sequence& from, const Sequence& to)
{
    auto pos = seq.rfind(from);
    if (pos != Sequence::npos)
        seq.replace(pos, from.length(), to);
}

template<typename Sequence>
Sequence replaceRightCopy(const Sequence& seq, const Sequence& from, const Sequence& to)
{
    Sequence copy(seq);
    replaceRight(copy, from, to);
    
    return copy;
}

template<typename Sequence>
void replace(Sequence& seq, const Sequence& from, const Sequence& to)
{
    auto pos = 0ull;
    while ((pos = seq.find(from, pos)) != Sequence::npos)
    {
        seq.replace(pos, from.length(), to);
        pos += to.length();
    }
}

template<typename Sequence>
Sequence replaceCopy(const Sequence& seq, const Sequence& from, const Sequence& to)
{
    Sequence copy(seq);
    replace(copy, from, to);

    return copy;
}

template<typename Sequence>
std::vector<Sequence> explode(const Sequence& str, const Sequence& delim)
{
    auto pos = 0ull;
    auto end = str.find(delim);
    auto res = std::vector<Sequence>();

    while (end != Sequence::npos)
    {
        res.push_back(str.substr(pos, end - pos));
        pos = end + delim.length();
        end = str.find(delim, pos);
    }
    res.push_back(str.substr(pos, end));

    return res;
}

template<typename Range, typename Sequence>
Sequence implode(const Range& range, const Sequence& delim)
{
    auto stream = std::ostringstream();

    for (const auto& value : range)
    {
        if (&value != &*range.begin())
            stream << delim;

        stream << value;
    }
    return stream.str();
}

template<typename Iterator, typename Value>
bool contains(Iterator begin, Iterator end, const Value& value)
{
    return std::find(begin, end, value) != end;
}

template<typename Container, typename Value>
bool contains(const Container& container, const Value& value)
{
    return contains(std::begin(container), std::end(container), value);
}

}  // namespace eggcpt
