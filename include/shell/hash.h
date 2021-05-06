#pragma once

#include <shell/int.h>

namespace shell
{

inline u64 murmur(const void* key, u64 size, u64 seed)
{
    constexpr u64 m = 0xC6A4'A793'5BD1'E995;
    constexpr u64 r = 47;

    const u64* data = reinterpret_cast<const u64*>(key);
    const u64* last = data + size / 8;

    u64 h = seed ^ (size * m);

    while (data != last)
    {
        u64 k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const u8* remaining = reinterpret_cast<const u8*>(data);

    switch (size & 7)
    {
    case 7: h ^= static_cast<u64>(remaining[6]) << 48; [[fallthrough]];
    case 6: h ^= static_cast<u64>(remaining[5]) << 40; [[fallthrough]];
    case 5: h ^= static_cast<u64>(remaining[4]) << 32; [[fallthrough]];
    case 4: h ^= static_cast<u64>(remaining[3]) << 24; [[fallthrough]];
    case 3: h ^= static_cast<u64>(remaining[2]) << 16; [[fallthrough]];
    case 2: h ^= static_cast<u64>(remaining[1]) <<  8; [[fallthrough]];
    case 1: h ^= static_cast<u64>(remaining[0]) <<  0;
            h *= m;
    }

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

template<typename T>
u64 hash(const T* data, u64 size)
{
    return murmur(data, size, 0);
}

template<typename T>
u64 hash(const T& value)
{
    return hash(&value, sizeof(T));
}

template<typename Range>
u64 hashRange(const Range& range)
{
    u64 seed = 0;
    for (const auto& value : range)
        seed = murmur(&value, sizeof(value), seed);

    return seed;
}

}  // namespace shell
