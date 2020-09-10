#pragma once

#include <eggcpt/log/none.h>
#include <eggcpt/log/sinks.h>

#ifdef EGGCPT_LOG_FATAL
#undef EGGCPT_LOG_FATAL
#endif

#define EGGCPT_LOG_FATAL(...) EGGCPT_LOG("[F]", eggcpt::Level::Fatal, __VA_ARGS__)
