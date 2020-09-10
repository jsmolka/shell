#pragma once

#include <eggcpt/log/none.h>
#include <eggcpt/log/sinks.h>

#ifdef EGGCPT_LOG_ERROR
#undef EGGCPT_LOG_ERROR
#endif

#define EGGCPT_LOG_ERROR(...) EGGCPT_LOG("[E]", eggcpt::Level::Error, __VA_ARGS__)
