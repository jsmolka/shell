#pragma once

#include <limits>
#include <type_traits>
#include <eggcpt/detail/base/int.h>

namespace eggcpt::bit
{

template<typename T>
constexpr base::uint bits()
{
    static_assert(std::is_integral_v<T>);

    return std::numeric_limits<std::make_unsigned_t<T>>::digits;
}

}  // namespace eggcpt::bit
