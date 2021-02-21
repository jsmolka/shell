#pragma once

#include <shell/log/none.h>
#include <shell/log/sinks.h>

#ifdef SHELL_LOG_FATAL
#  undef SHELL_LOG_FATAL
#endif

#define SHELL_LOG_FATAL(...) SHELL_LOG(shell::Level::Fatal, __VA_ARGS__)
