#pragma warning(push)
#pragma warning(disable:6319)
#pragma warning(disable:6237)

#include <iostream>
#include <string>
#include <vector>

#define EGGCPT_LOG_LEVEL_DEBUG
#include <eggcpt/algorithm.h>
#include <eggcpt/bit.h>
#include <eggcpt/filesystem.h>
#include <eggcpt/logging.h>
#include <eggcpt/options.h>
#include <eggcpt/predicates.h>
#include <eggcpt/singleton.h>
#include <eggcpt/traits.h>
#include <eggcpt/utility.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace eggcpt;
using namespace std::string_literals;

#include "tests_algorithm.inl"
#include "tests_bit.inl"
#include "tests_filesystem.inl"
#include "tests_logging.inl"
#include "tests_options.inl"
#include "tests_predicates.inl"
#include "tests_singleton.inl"
#include "tests_traits.inl"
#include "tests_utility.inl"

#pragma warning(pop)
