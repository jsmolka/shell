#pragma once

#include <shell/log/none.h>
#include <shell/log/sinks.h>

#ifdef SHELL_LOG_DEBUG
#  undef SHELL_LOG_DEBUG
#endif

#define SHELL_LOG_DEBUG(...) SHELL_LOG(shell::Level::Debug, __VA_ARGS__)
