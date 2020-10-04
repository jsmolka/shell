#pragma once

#include <shell/log/none.h>
#include <shell/log/sinks.h>

#ifdef SHELL_LOG_ERROR
#undef SHELL_LOG_ERROR
#endif

#define SHELL_LOG_ERROR(...) SHELL_LOG("[E]", shell::Level::Error, __VA_ARGS__)
