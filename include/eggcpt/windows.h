#pragma once

#include <eggcpt/macro.h>

#if EGGCPT_OS_WINDOWS
#  ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#  endif
#
#  ifndef NOMINMAX
#  define NOMINMAX
#  endif
#
#  include <windows.h>
#
#  undef NOMINMAX
#  undef WIN32_LEAN_AND_MEAN
#endif

