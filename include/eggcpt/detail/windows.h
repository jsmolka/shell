#pragma once

#include <eggcpt/detail/macro/platform.h>

#if EGGCPT_PLATFORM_WINDOWS
#  if !defined(NOMINMAX) && !defined(WIN32_LEAN_AND_MEAN)
#    define NOMINMAX
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    undef WIN32_LEAN_AND_MEAN
#    undef NOMINMAX
#  else
#    include <windows.h>
#  endif
#endif
