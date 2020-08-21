#pragma once

#ifndef FMT_HEADER_ONLY
#  define FMT_HEADER_ONLY
#  define FMT_HEADER_ONLY_DEFINED
#endif

#include <eggcpt/external/fmt/include/fmt/format.h>

#ifdef FMT_HEADER_ONLY_DEFINED
#  undef FMT_HEADER_ONLY
#  undef FMT_HEADER_ONLY_DEFINED
#endif

namespace eggcpt
{

template<typename String, typename... Args, typename Char = fmt::char_t<String>>
std::basic_string<Char> format(const String& format, Args&&... args)
{
    return fmt::format(format, std::forward<Args>(args)...);
}

}  // namespace eggcpt
