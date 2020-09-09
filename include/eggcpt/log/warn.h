#pragma once

#include <eggcpt/log/none.h>

#ifdef EGGCPT_LOG_WARN
#undef EGGCPT_LOG_WARN
#endif

#define EGGCPT_LOG_WARN(...) EGGCPT_LOG("[W]", eggcpt::Level::Warn, __VA_ARGS__)
