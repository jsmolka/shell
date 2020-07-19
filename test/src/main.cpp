#pragma warning(push)
#pragma warning(disable:6319)
#pragma warning(disable:6237)

#define EGGCPT_LOG_LEVEL_DEBUG

#include <vector>
#include <eggcpt/bit.h>
#include <eggcpt/logging.h>
#include <eggcpt/mpl.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace eggcpt;

#include "tests_bit.inl"
#include "tests_mpl.inl"
#include "tests_logging.inl"

#pragma warning(pop)
