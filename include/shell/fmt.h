#pragma once

#ifndef FMT_HEADER_ONLY
#  define FMT_HEADER_ONLY
#  define FMT_HEADER_ONLY_DEFINED
#endif

#include <shell/detail/fmt/chrono.h>
#include <shell/detail/fmt/color.h>
#include <shell/detail/fmt/compile.h>
#include <shell/detail/fmt/core.h>
#include <shell/detail/fmt/format.h>
#include <shell/detail/fmt/locale.h>
#include <shell/detail/fmt/os.h>
#include <shell/detail/fmt/ostream.h>
#include <shell/detail/fmt/printf.h>
#include <shell/detail/fmt/ranges.h>

#ifdef FMT_HEADER_ONLY_DEFINED
#  undef FMT_HEADER_ONLY
#  undef FMT_HEADER_ONLY_DEFINED
#endif
