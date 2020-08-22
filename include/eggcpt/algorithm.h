#pragma once

#include <algorithm>
#include <vector>

#include <eggcpt/locale.h>
#include <eggcpt/traits.h>
#include <eggcpt/utility.h>

namespace eggcpt
{

namespace detail
{

template<typename Sequence>
std::size_t size(const Sequence& seq)
{
    if constexpr (std::is_array_v<Sequence>)
        return sizeof(seq) - 1;
    else
        return seq.size();
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
    trimLeftIf(seq, IsSpace<range_value_t<Sequence>>(locale));
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

template<typename Sequence>
Sequence trimLeftCopy(const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimLeftCopyIf(seq, IsSpace<range_value_t<Sequence>>(locale));
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
    trimRightIf(seq, IsSpace<range_value_t<Sequence>>(locale));
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

template<typename Sequence>
Sequence trimRightCopy(const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimRightCopyIf(seq, IsSpace<range_value_t<Sequence>>(locale));
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
    trimIf(seq, IsSpace<range_value_t<Sequence>>(locale));
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

template<typename Sequence>
Sequence trimCopy(const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimCopyIf(seq, IsSpace<range_value_t<Sequence>>(locale));
}

template<typename Sequence>
void toLower(Sequence& seq, const std::locale& locale = std::locale())
{
    std::transform(
        std::begin(seq),
        std::end(seq),
        std::begin(seq),
        ToLower<range_value_t<Sequence>>(locale));
}

template<typename Sequence>
Sequence toLowerCopy(const Sequence& seq, const std::locale& locale = std::locale())
{
    Sequence res;
    res.reserve(detail::size(seq));

    std::transform(
        std::begin(seq),
        std::end(seq),
        std::back_inserter(res),
        ToLower<range_value_t<Sequence>>(locale));

    return res;
}

template<typename Sequence>
void toUpper(Sequence& seq, const std::locale& locale = std::locale())
{
    std::transform(
        std::begin(seq),
        std::end(seq),
        std::begin(seq),
        ToUpper<range_value_t<Sequence>>(locale));
}

template<typename Sequence>
Sequence toUpperCopy(const Sequence& seq, const std::locale& locale = std::locale())
{
    Sequence res;
    res.reserve(detail::size(seq));

    std::transform(
        std::begin(seq),
        std::end(seq),
        std::back_inserter(res),
        ToUpper<range_value_t<Sequence>>(locale));

    return res;
}

template<typename Sequence, typename SequenceFrom, typename SequenceTo>
void replaceFirst(Sequence& seq, const SequenceFrom& from, const SequenceTo& to)
{
    auto pos = seq.find(from);
    if ( pos != Sequence::npos)
    {
        seq.replace(pos, detail::size(from), to);
    }
}

template<typename Sequence, typename SequenceFrom, typename SequenceTo>
Sequence replaceFirstCopy(const Sequence& seq, const SequenceFrom& from, const SequenceTo& to)
{
    Sequence res(seq);
    replaceFirst(res, from, to);
    
    return res;
}

template<typename Sequence, typename SequenceFrom, typename SequenceTo>
void replaceLast(Sequence& seq, const SequenceFrom& from, const SequenceTo& to)
{
    auto pos = seq.rfind(from);
    if ( pos != Sequence::npos)
    {
        seq.replace(pos, detail::size(from), to);
    }
}

template<typename Sequence, typename SequenceFrom, typename SequenceTo>
Sequence replaceLastCopy(const Sequence& seq, const SequenceFrom& from, const SequenceTo& to)
{
    Sequence res(seq);
    replaceLast(res, from, to);
    
    return res;
}

template<typename Sequence, typename SequenceFrom, typename SequenceTo>
void replaceAll(Sequence& seq, const SequenceFrom& from, const SequenceTo& to)
{
    typename Sequence::size_type pos = 0;
    while ((pos = seq.find(from, pos)) != Sequence::npos)
    {
        seq.replace(pos, detail::size(from), to);
        pos += detail::size(to);
    }
}

template<typename Sequence, typename SequenceFrom, typename SequenceTo>
Sequence replaceAllCopy(const Sequence& seq, const SequenceFrom& from, const SequenceTo& to)
{
    Sequence res(seq);
    replaceAll(res, from, to);

    return res;
}

template<typename Sequence, typename SequenceDelimiter>
std::vector<Sequence> split(const Sequence& seq, const SequenceDelimiter& del)
{
    auto end = seq.find(del);
    auto res = std::vector<Sequence>();
    auto pos = static_cast<typename Sequence::size_type>(0);

    while (end != Sequence::npos)
    {
        res.push_back(seq.substr(pos, end - pos));
        pos = end + detail::size(del);
        end = seq.find(del, pos);
    }
    res.push_back(seq.substr(pos, end));

    return res;
}

template<typename SequenceRange, typename SequenceDelimiter>
range_value_t<SequenceRange> join(const SequenceRange& range, const SequenceDelimiter& del)
{
    range_value_t<SequenceRange> res;

    for (auto [pos, seq] : enumerate(range))
    {
        if (pos != 0)
            res.append(del);

        res.append(seq);
    }
    return res;
}

}  // namespace eggcpt
