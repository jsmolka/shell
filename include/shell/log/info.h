#pragma once

#include <shell/log/none.h>
#include <shell/log/sinks.h>

#ifdef SHELL_LOG_INFO
#  undef SHELL_LOG_INFO
#endif

#define SHELL_LOG_INFO(...) SHELL_LOG("[I]", shell::Level::Info, __VA_ARGS__)
