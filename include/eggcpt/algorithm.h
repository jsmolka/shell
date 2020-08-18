#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "predicates.h"

namespace eggcpt
{

template<typename Sequence, typename Predicate>
void trimLeftIf(Sequence& seq, Predicate pred)
{
    seq.erase(seq.begin(), std::find_if_not(seq.begin(), seq.end(), pred));
}

template<typename Sequence, typename Predicate>
Sequence trimLeftIfCopy(const Sequence& seq, Predicate pred)
{
    Sequence copy(seq);
    trimLeftIf(copy, pred);

    return copy;
}

template<typename Sequence>
void trimLeft(Sequence& seq)
{
    trimLeftIf(seq, isspace);
}

template<typename Sequence>
Sequence trimLeftCopy(const Sequence& seq)
{
    Sequence copy(seq);
    trimLeft(copy);

    return copy;
}

template<typename Sequence, typename Predicate>
void trimRightIf(Sequence& seq, Predicate pred)
{
    seq.erase(std::find_if_not(seq.rbegin(), seq.rend(), pred).base(), seq.end());
}

template<typename Sequence, typename Predicate>
Sequence trimRightIfCopy(const Sequence& seq, Predicate pred)
{
    Sequence copy(seq);
    trimRightIf(copy, pred);

    return copy;
}

template<typename Sequence>
void trimRight(Sequence& seq)
{
    trimRightIf(seq, isspace);
}

template<typename Sequence>
Sequence trimRightCopy(const Sequence& seq)
{
    Sequence copy(seq);
    trimRight(copy);

    return copy;
}

template<typename Sequence, typename Predicate>
void trimIf(Sequence& seq, Predicate pred)
{
    trimLeftIf(seq, pred);
    trimRightIf(seq, pred);
}

template<typename Sequence, typename Predicate>
Sequence trimIfCopy(const Sequence& seq, Predicate pred)
{
    Sequence copy(seq);
    trimIf(copy, pred);

    return copy;
}

template<typename Sequence>
void trim(Sequence& seq)
{
    trimIf(seq, isspace);
}

template<typename Sequence>
Sequence trimCopy(const Sequence& seq)
{
    Sequence copy(seq);
    trim(copy);

    return copy;
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
