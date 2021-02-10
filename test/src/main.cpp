#include <shell/algorithm.h>
#include <shell/bit.h>
#include <shell/buffer.h>
#include <shell/errors.h>
#include <shell/filesystem.h>
#include <shell/ini.h>
#include <shell/int.h>
#include <shell/locale.h>
#include <shell/log/all.h>
#include <shell/options.h>
#include <shell/ranges.h>
#include <shell/ringbuffer.h>
#include <shell/traits.h>
#include <shell/utility.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace shell;
using namespace std::string_literals;

#include "tests_algorithm.inl"
#include "tests_bit.inl"
#include "tests_buffer.inl"
#include "tests_errors.inl"
#include "tests_filesystem.inl"
#include "tests_ini.inl"
#include "tests_locale.inl"
#include "tests_log.inl"
#include "tests_options.inl"
#include "tests_parse.inl"
#include "tests_ranges.inl"
#include "tests_ringbuffer.inl"
#include "tests_traits.inl"
#include "tests_utility.inl"
