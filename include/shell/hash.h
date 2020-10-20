#pragma once

#include <utility>

namespace shell
{

namespace hash
{

template<typename T>
void hashCombine(std::size_t& seed, const T& value)
{
    seed ^= std::hash<T>()(value) + 0x9E37'79B9 + (seed << 6) + (seed >> 2);
}

template<typename Iterator>
std::size_t hashRange(Iterator begin, Iterator end)
{
    std::size_t seed = 0;
    for (; begin != end; ++begin)
        hashCombine(seed, *begin);

    return seed;
}

}  // namespace hash

using hash::hashCombine;
using hash::hashRange;

}  // namespace shell
