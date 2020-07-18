#pragma once

#include <eggcpt/detail/base/int.h>

namespace eggcpt::bit
{

template<unsigned Size> struct storage_type {};
template<> struct storage_type<1> { using type = base::u8;  };
template<> struct storage_type<2> { using type = base::u16; };
template<> struct storage_type<4> { using type = base::u32; };
template<> struct storage_type<8> { using type = base::u64; };

template<unsigned Size>
using storage_type_t = typename storage_type<Size>::type;

}  // namespace eggcpt::bit
