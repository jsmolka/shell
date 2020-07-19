#pragma once

namespace eggcpt::logging
{

enum level
{
    kLevelDebug = 0,
    kLevelInfo  = 1,
    kLevelWarn  = 2,
    kLevelError = 3,
    kLevelFatal = 4,
    kLevelNone  = 5
};

#ifdef EGGCPT_LOG_LEVEL_DEBUG
#  define EGGCPT_LOG_LEVEL 0
#elif defined(EGGCPT_LOG_LEVEL_INFO)
#  define EGGCPT_LOG_LEVEL 1
#elif defined(EGGCPT_LOG_LEVEL_WARN)
#  define EGGCPT_LOG_LEVEL 2
#elif defined(EGGCPT_LOG_LEVEL_ERROR)
#  define EGGCPT_LOG_LEVEL 3
#elif defined(EGGCPT_LOG_LEVEL_FATAL)
#  define EGGCPT_LOG_LEVEL 4
#elif defined(EGGCPT_LOG_LEVEL_NONE)
#  define EGGCPT_LOG_LEVEL 5
#else
#  error Log level is not defined (must be EGGCPT_LOG_LEVEL_INFO / WARN / ERROR / FATAL / NONE)
#endif

constexpr level kLevel = static_cast<level>(EGGCPT_LOG_LEVEL);

}
