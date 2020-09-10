#pragma once

#include <eggcpt/log/none.h>
#include <eggcpt/log/sinks.h>

#ifdef EGGCPT_LOG_DEBUG
#undef EGGCPT_LOG_DEBUG
#endif

#define EGGCPT_LOG_DEBUG(...) EGGCPT_LOG("[D]", eggcpt::Level::Debug, __VA_ARGS__)
