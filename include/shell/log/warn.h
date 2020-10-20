#pragma once

#include <shell/log/none.h>
#include <shell/log/sinks.h>

#ifdef SHELL_LOG_WARN
#  undef SHELL_LOG_WARN
#endif

#define SHELL_LOG_WARN(...) SHELL_LOG("[W]", shell::Level::Warn, __VA_ARGS__)
