#pragma once

#include <eggcpt/log/sinks.h>

#define EGGCPT_LOG_DEBUG(...) static_cast<void>(0)
#define EGGCPT_LOG_INFO(...)  static_cast<void>(0)
#define EGGCPT_LOG_WARN(...)  static_cast<void>(0)
#define EGGCPT_LOG_ERROR(...) static_cast<void>(0)
#define EGGCPT_LOG_FATAL(...) static_cast<void>(0)
