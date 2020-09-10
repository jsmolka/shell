#pragma once

#include <eggcpt/log/none.h>
#include <eggcpt/log/sinks.h>

#ifdef EGGCPT_LOG_INFO
#undef EGGCPT_LOG_INFO
#endif

#define EGGCPT_LOG_INFO(...) EGGCPT_LOG("[I]", eggcpt::Level::Info, __VA_ARGS__)
