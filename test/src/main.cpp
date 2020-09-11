#include <eggcpt/algorithm.h>
#include <eggcpt/bit.h>
#include <eggcpt/bitfield.h>
#include <eggcpt/errors.h>
#include <eggcpt/filesystem.h>
#include <eggcpt/iterator.h>
#include <eggcpt/locale.h>
#include <eggcpt/log/all.h>
#include <eggcpt/options.h>
#include <eggcpt/traits.h>
#include <eggcpt/utility.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace eggcpt;
using namespace std::string_literals;

#include "tests_algorithm.inl"
#include "tests_bit.inl"
#include "tests_bitfield.inl"
#include "tests_errors.inl"
#include "tests_filesystem.inl"
#include "tests_iterator.inl"
#include "tests_locale.inl"
#include "tests_log.inl"
#include "tests_options.inl"
#include "tests_traits.inl"
#include "tests_utility.inl"
