#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace eggcpt
{

namespace algorithm
{

template<typename Sequence, typename Predicate>
inline void trim_left_if(Sequence& seq, Predicate pred)
{
    seq.erase(seq.begin(), std::find_if_not(seq.begin(), seq.end(), pred));
}

template<typename Sequence, typename Predicate>
inline Sequence trim_left_if_copy(const Sequence& seq, Predicate pred)
{
    Sequence copy(seq);
    trim_left_if(copy, pred);

    return copy;
}

template<typename Sequence>
inline void trim_left(Sequence& seq)
{
    trim_left_if(seq, isspace);
}

template<typename Sequence>
inline Sequence trim_left_copy(const Sequence& seq)
{
    Sequence copy(seq);
    trim_left(copy);

    return copy;
}

template<typename Sequence, typename Predicate>
inline void trim_right_if(Sequence& seq, Predicate pred)
{
    seq.erase(std::find_if_not(seq.rbegin(), seq.rend(), pred).base(), seq.end());
}

template<typename Sequence, typename Predicate>
inline Sequence trim_right_if_copy(const Sequence& seq, Predicate pred)
{
    Sequence copy(seq);
    trim_right_if(copy, pred);

    return copy;
}

template<typename Sequence>
inline void trim_right(Sequence& seq)
{
    trim_right_if(seq, isspace);
}

template<typename Sequence>
inline Sequence trim_right_copy(const Sequence& seq)
{
    Sequence copy(seq);
    trim_right(copy);

    return copy;
}

template<typename Sequence, typename Predicate>
inline void trim_if(Sequence& seq, Predicate pred)
{
    trim_left_if(seq, pred);
    trim_right_if(seq, pred);
}

template<typename Sequence, typename Predicate>
inline Sequence trim_if_copy(const Sequence& seq, Predicate pred)
{
    Sequence copy(seq);
    trim_if(copy, pred);

    return copy;
}

template<typename Sequence>
inline void trim(Sequence& seq)
{
    trim_if(seq, isspace);
}

template<typename Sequence>
inline Sequence trim_copy(const Sequence& seq)
{
    Sequence copy(seq);
    trim(copy);

    return copy;
}

template<typename Sequence>
inline void replace_left(Sequence& seq, const Sequence& from, const Sequence& to)
{
    auto pos = seq.find(from);
    if (pos != Sequence::npos)
        seq.replace(pos, from.length(), to);
}

template<typename Sequence>
inline Sequence replace_left_copy(const Sequence& seq, const Sequence& from, const Sequence& to)
{
    Sequence copy(seq);
    replace_left(copy, from, to);
    
    return copy;
}

template<typename Sequence>
inline void replace_right(Sequence& seq, const Sequence& from, const Sequence& to)
{
    auto pos = seq.rfind(from);
    if (pos != Sequence::npos)
        seq.replace(pos, from.length(), to);
}

template<typename Sequence>
inline Sequence replace_right_copy(const Sequence& seq, const Sequence& from, const Sequence& to)
{
    Sequence copy(seq);
    replace_right(copy, from, to);
    
    return copy;
}

template<typename Sequence>
inline void replace(Sequence& seq, const Sequence& from, const Sequence& to)
{
    auto pos = 0ull;

    while ((pos = seq.find(from, pos)) != Sequence::npos)
    {
        seq.replace(pos, from.length(), to);
        pos += to.length();
    }
}

template<typename Sequence>
inline Sequence replace_copy(const Sequence& seq, const Sequence& from, const Sequence& to)
{
    Sequence copy(seq);
    replace(copy, from, to);

    return copy;
}

template<typename Sequence>
inline std::vector<Sequence> explode(const Sequence& str, const Sequence& delim)
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

template<typename Container, typename Sequence>
inline Sequence implode(const Container& tokens, const Sequence& delim)
{
    auto stream = std::ostringstream();

    for (const auto& token : tokens)
    {
        if (&token != &*tokens.begin())
            stream << delim;

        stream << token;
    }
    return stream.str();
}

}  // namespace algorithm

using algorithm::trim_left_if;
using algorithm::trim_left_if_copy;
using algorithm::trim_left;
using algorithm::trim_left_copy;
using algorithm::trim_right_if;
using algorithm::trim_right_if_copy;
using algorithm::trim_right;
using algorithm::trim_right_copy;
using algorithm::trim_if;
using algorithm::trim_if_copy;
using algorithm::trim;
using algorithm::trim_copy;
using algorithm::replace_left;
using algorithm::replace_left_copy;
using algorithm::replace_right;
using algorithm::replace_right_copy;
using algorithm::replace;
using algorithm::replace_copy;
using algorithm::explode;
using algorithm::implode;

}  // namespace eggcpt
