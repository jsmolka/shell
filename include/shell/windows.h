#pragma once

#include <shell/predef.h>

#if SHELL_OS_WINDOWS
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN_DEFINED
#  endif
#
#  ifndef NOMINMAX
#    define NOMINMAX
#    define NOMINMAX_DEFINED
#  endif
#
#  include <windows.h>
#
#  ifdef NOMINMAX_DEFINED
#    undef NOMINMAX
#    undef NOMINMAX_DEFINED
#  endif
#
#  ifdef WIN32_LEAN_AND_MEAN_DEFINED
#    undef WIN32_LEAN_AND_MEAN
#    undef WIN32_LEAN_AND_MEAN_DEFINED
#  endif
#endif
