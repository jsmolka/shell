#pragma once

#include <cstring>
#include <shell/int.h>

namespace shell
{

template<typename T, typename U>
T read(const U* data, std::size_t index)
{
    return *reinterpret_cast<const T*>(reinterpret_cast<const u8*>(data) + index);
}

template<typename T, typename U>
void write(U* data, std::size_t index, const T& value)
{
    *reinterpret_cast<T*>(reinterpret_cast<u8*>(data) + index) = value;
}

}  // namespace shell
