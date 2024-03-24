#include <shell/algorithm.h>
#include <shell/array.h>
#include <shell/bit.h>
#include <shell/errors.h>
#include <shell/filesystem.h>
#include <shell/hash.h>
#include <shell/ini.h>
#include <shell/int.h>
#include <shell/locale.h>
#include <shell/log/all.h>
#include <shell/main.h>
#include <shell/macros.h>
#include <shell/mp.h>
#include <shell/operators.h>
#include <shell/options.h>
#include <shell/punning.h>
#include <shell/ranges.h>
#include <shell/ringbuffer.h>
#include <shell/stack.h>
#include <shell/traits.h>
#include <shell/utility.h>
#include <shell/vector.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace shell;
using namespace std::string_literals;

#include "tests_algorithm.inl"
#include "tests_array.inl"
#include "tests_bit.inl"
#include "tests_errors.inl"
#include "tests_filesystem.inl"
#include "tests_hash.inl"
#include "tests_ini.inl"
#include "tests_locale.inl"
#include "tests_log.inl"
#include "tests_operators.inl"
#include "tests_options.inl"
#include "tests_macros.inl"
#include "tests_mp.inl"
#include "tests_parse.inl"
#include "tests_punning.inl"
#include "tests_ranges.inl"
#include "tests_ringbuffer.inl"
#include "tests_stack.inl"
#include "tests_traits.inl"
#include "tests_utility.inl"
#include "tests_vector.inl"
