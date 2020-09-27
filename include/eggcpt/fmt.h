#pragma once

#ifndef FMT_HEADER_ONLY
#  define FMT_HEADER_ONLY
#  define FMT_HEADER_ONLY_DEFINED
#endif

#include <eggcpt/external/fmt/include/fmt/core.h>
#include <eggcpt/external/fmt/include/fmt/format.h>
#include <eggcpt/external/fmt/include/fmt/color.h>

#ifdef FMT_HEADER_ONLY_DEFINED
#  undef FMT_HEADER_ONLY
#  undef FMT_HEADER_ONLY_DEFINED
#endif

#include <eggcpt/filesystem.h>

template<>
struct fmt::formatter<eggcpt::filesystem::path>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const eggcpt::filesystem::path& path, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "{}", path.string());
    }
};

