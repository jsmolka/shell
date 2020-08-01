#pragma once

#include <array>
#include <fstream>
#include <memory>
#include <type_traits>

#include "ansi.h"
#include "filesystem.h"

namespace eggcpt
{

namespace logging
{

enum class level { debug, info, warn, error, fatal };

class basic_sink
{
public:
    virtual ~basic_sink() = default;

    virtual void sink(const std::string& message, level level) = 0;
};

class console_sink : public basic_sink
{
public:
    virtual void sink(const std::string& message, level) override
    {
        fmt::print(message);
    }
};

class colored_console_sink : public console_sink
{
public:
    virtual void sink(const std::string& message, level level) override
    {
        color color = color::white;
        switch (level)
        {
        case logging::level::debug: color = color::cyan;    break;
        case logging::level::info:  color = color::green;   break;
        case logging::level::warn:  color = color::yellow;  break;
        case logging::level::error: color = color::red;     break;
        case logging::level::fatal: color = color::magenta; break;

        default:
            EGGCPT_UNREACHABLE;
            break;
        }
        console_sink::sink(escape(message, color), level);
    }
};

class file_sink : public basic_sink
{
public:
    file_sink(const filesystem::path& file, bool trunc = false)
        : stream{ filesystem::make_absolute(file),
            trunc ? std::ios::trunc : std::ios::app } {}

    virtual void sink(const std::string& message, level) override
    {
        if (stream.is_open())
            stream << message;
    }

private:
    std::ofstream stream;
};

template<typename... Ts>
class multi_sink : public basic_sink
{
    static_assert(sizeof...(Ts) > 0);

public:
    multi_sink(Ts&&... sinks)
        : sinks{ std::make_shared<Ts>(std::move(sinks))... } {}

    virtual void sink(const std::string& message, level level) override
    {
        for (const auto& sink : sinks)
            sink->sink(message, level);
    }

private:
    std::array<std::shared_ptr<basic_sink>, sizeof...(Ts)> sinks;
};

auto& default_sink()
{
    static std::shared_ptr<basic_sink> sink = std::make_shared<colored_console_sink>();

    return sink;
}

template<typename T>
void set_default_sink(const T& sink)
{
    static_assert(std::is_base_of_v<basic_sink, T>);

    default_sink() = std::make_shared<T>(sink);
}

}  // namespace logging

using logging::basic_sink;
using logging::console_sink;
using logging::colored_console_sink;
using logging::file_sink;
using logging::multi_sink;
using logging::set_default_sink;

}  // namespace eggcpt

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
#else
#  define EGGCPT_LOG_LEVEL 5
#endif

#define EGGCPT_LOG(prefix, value, ...)                                  \
    eggcpt::logging::default_sink()->sink(                              \
        fmt::format(prefix" {}:{} :: {}\n", EGGCPT_FUNCTION, __LINE__,  \
            fmt::format(__VA_ARGS__)), eggcpt::logging::level::value)

#if EGGCPT_LOG_LEVEL <= 0
#  define EGGCPT_LOG_DEBUG(...) EGGCPT_LOG("[D]", debug, __VA_ARGS__)
#else
#  define EGGCPT_LOG_DEBUG(...) static_cast<void>(0)
#endif

#if EGGCPT_LOG_LEVEL <= 1
#  define EGGCPT_LOG_INFO(...)  EGGCPT_LOG("[I]", info , __VA_ARGS__)
#else
#  define EGGCPT_LOG_INFO(...)  static_cast<void>(0)
#endif

#if EGGCPT_LOG_LEVEL <= 2
#  define EGGCPT_LOG_WARN(...)  EGGCPT_LOG("[W]", warn , __VA_ARGS__)
#else
#  define EGGCPT_LOG_WARN(...)  static_cast<void>(0)
#endif

#if EGGCPT_LOG_LEVEL <= 3
#  define EGGCPT_LOG_ERROR(...) EGGCPT_LOG("[E]", error, __VA_ARGS__)
#else
#  define EGGCPT_LOG_ERROR(...) static_cast<void>(0)
#endif

#if EGGCPT_LOG_LEVEL <= 4
#  define EGGCPT_LOG_FATAL(...) EGGCPT_LOG("[F]", fatal, __VA_ARGS__)
#else
#  define EGGCPT_LOG_FATAL(...) static_cast<void>(0)
#endif
