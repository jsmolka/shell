#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include <eggcpt/locale.h>
#include <eggcpt/traits.h>

namespace eggcpt
{

namespace detail
{

template<typename Range>
std::size_t rangeSize(const Range& range)
{
    return std::distance(std::begin(range), std::end(range));
}

template<typename Range, std::size_t N>
std::size_t rangeSize(const Range (&range)[N])
{
    static_assert(is_any_of_v<std::decay_t<Range>, char, wchar_t>);

    return N - 1;
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

template<typename OutputIterator, typename Sequence>
OutputIterator trimLeftCopy(OutputIterator output, const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimLeftCopyIf(output, seq, IsSpace<range_value_t<const Sequence>>(locale));
}

template<typename Sequence>
Sequence trimLeftCopy(const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimLeftCopyIf(seq, IsSpace<range_value_t<const Sequence>>(locale));
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
    return trimRightCopyIf(seq, IsSpace<range_value_t<const Sequence>>(locale));
}

template<typename OutputIterator, typename Sequence>
OutputIterator trimRightCopy(OutputIterator output, const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimRightCopyIf(output, seq, IsSpace<range_value_t<const Sequence>>(locale));
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

template<typename OutputIterator, typename Sequence>
OutputIterator trimCopy(OutputIterator output, const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimCopyIf(output, seq, IsSpace<range_value_t<const Sequence>>(locale));
}

template<typename Sequence>
Sequence trimCopy(const Sequence& seq, const std::locale& locale = std::locale())
{
    return trimCopyIf(seq, IsSpace<range_value_t<const Sequence>>(locale));
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

template<typename OutputIterator, typename Sequence>
OutputIterator toLowerCopy(OutputIterator output, const Sequence& seq, const std::locale& locale = std::locale())
{
    return std::transform(
        std::begin(seq),
        std::end(seq),
        output,
        ToLower<range_value_t<const Sequence>>(locale));
}

template<typename Sequence>
Sequence toLowerCopy(const Sequence& seq, const std::locale& locale = std::locale())
{
    Sequence result;
    result.reserve(seq.size());
    toLowerCopy(std::back_inserter(result), seq, locale);

    return result;
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

template<typename OutputIterator, typename Sequence>
OutputIterator toUpperCopy(OutputIterator output, const Sequence& seq, const std::locale& locale = std::locale())
{
    return std::transform(
        std::begin(seq),
        std::end(seq),
        output,
        ToUpper<range_value_t<const Sequence>>(locale));
}

template<typename Sequence>
Sequence toUpperCopy(const Sequence& seq, const std::locale& locale = std::locale())
{
    Sequence result;
    result.reserve(seq.size());
    toUpperCopy(std::back_inserter(result), seq, locale);

    return result;
}

template<typename Sequence, typename RangeFrom, typename RangeTo>
void replaceFirst(Sequence& seq, const RangeFrom& from, const RangeTo& to)
{
    auto pos = seq.find(from);
    if (pos != Sequence::npos)
        seq.replace(pos, detail::rangeSize(from), to);
}

template<typename Sequence, typename RangeFrom, typename RangeTo>
Sequence replaceFirstCopy(const Sequence& seq, const RangeFrom& from, const RangeTo& to)
{
    Sequence result(seq);
    replaceFirst(result, from, to);
    
    return result;
}

template<typename Sequence, typename RangeFrom, typename RangeTo>
void replaceLast(Sequence& seq, const RangeFrom& from, const RangeTo& to)
{
    auto pos = seq.rfind(from);
    if (pos != Sequence::npos)
        seq.replace(pos, detail::rangeSize(from), to);
}

template<typename Sequence, typename RangeFrom, typename RangeTo>
Sequence replaceLastCopy(const Sequence& seq, const RangeFrom& from, const RangeTo& to)
{
    Sequence result(seq);
    replaceLast(result, from, to);
    
    return result;
}

template<typename Sequence, typename RangeFrom, typename RangeTo>
void replaceAll(Sequence& seq, const RangeFrom& from, const RangeTo& to)
{
    auto pos = static_cast<typename Sequence::size_type>(0);
    while ((pos = seq.find(from, pos)) != Sequence::npos)
    {
        seq.replace(pos, detail::rangeSize(from), to);
        pos += detail::rangeSize(to);
    }
}

template<typename Sequence, typename RangeFrom, typename RangeTo>
Sequence replaceAllCopy(const Sequence& seq, const RangeFrom& from, const RangeTo& to)
{
    Sequence result(seq);
    replaceAll(result, from, to);

    return result;
}

template<typename Sequence, typename Range>
std::vector<Sequence> split(const Sequence& str, const Range& delim)
{
    auto end = str.find(delim);
    auto res = std::vector<Sequence>();
    auto pos = static_cast<typename Sequence::size_type>(0);

    while (end != Sequence::npos)
    {
        res.push_back(str.substr(pos, end - pos));
        pos = end + detail::rangeSize(delim);
        end = str.find(delim, pos);
    }
    res.push_back(str.substr(pos, end));

    return res;
}

template<typename SequenceRange, typename Range>
range_value_t<SequenceRange> join(const SequenceRange& range, const Range& delim)
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

}  // namespace eggcpt
