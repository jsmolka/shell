#pragma once

#ifndef FMT_HEADER_ONLY
#  define FMT_HEADER_ONLY
#  define FMT_HEADER_ONLY_DEFINED
#endif

#include <shell/external/fmt/include/fmt/core.h>
#include <shell/external/fmt/include/fmt/format.h>
#include <shell/external/fmt/include/fmt/color.h>

#ifdef FMT_HEADER_ONLY_DEFINED
#  undef FMT_HEADER_ONLY
#  undef FMT_HEADER_ONLY_DEFINED
#endif
