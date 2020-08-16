#pragma once

#include <array>
#include <fstream>
#include <memory>
#include <type_traits>

#include "filesystem.h"
#include "fmt.h"
#include "primitives.h"
#include "windows.h"

namespace eggcpt
{

enum class Level { Debug, Info, Warn, Error, Fatal };

class Sink
{
public:
    virtual ~Sink() = default;

    virtual void sink(const std::string& message, Level level) = 0;
};

class ConsoleSink : public Sink
{
public:
    virtual void sink(const std::string& message, Level) override
    {
        fmt::print(message);
    }
};

class ColoredConsoleSink : public ConsoleSink
{
public:
    ColoredConsoleSink()
    {
        #if EGGCPT_OS_WINDOWS
        auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (handle != INVALID_HANDLE_VALUE)
        {
            DWORD mode;
            if (GetConsoleMode(handle, &mode))
            {
                mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(handle, mode);
            }
        }
        #endif
    }

    virtual void sink(const std::string& message, Level level) override
    {
        uint color = 0;
        switch (level)
        {
        case Level::Debug: color = 96; break;  // Cyan
        case Level::Info:  color = 92; break;  // Green
        case Level::Warn:  color = 93; break;  // Yellow
        case Level::Error: color = 91; break;  // Red
        case Level::Fatal: color = 95; break;  // Magenta
        }
        ConsoleSink::sink(fmt::format("\033[{}m{}\033[0m", color, message), level);
    }
};

class FileSink : public Sink
{
public:
    FileSink(const filesystem::path& file, bool trunc = false)
        : stream(filesystem::make_absolute(file),
            trunc ? std::ios::trunc : std::ios::app) {}

    virtual void sink(const std::string& message, Level) override
    {
        if (stream.is_open())
            stream << message;
    }

private:
    std::ofstream stream;
};

template<typename... Ts>
class MultiSink : public Sink
{
    static_assert(sizeof...(Ts) > 0);

public:
    MultiSink(Ts&&... sinks)
        : sinks({ std::make_shared<Ts>(std::forward<Ts>(sinks))... }) {}

    virtual void sink(const std::string& message, Level level) override
    {
        for (const auto& sink : sinks)
            sink->sink(message, level);
    }

private:
    std::array<std::shared_ptr<Sink>, sizeof...(Ts)> sinks;
};

std::shared_ptr<Sink>& default_sink()
{
    static std::shared_ptr<Sink> sink = std::make_shared<ColoredConsoleSink>();

    return sink;
}

template<typename T>
void set_default_sink(const T& sink)
{
    static_assert(std::is_base_of_v<Sink, T>);

    default_sink() = std::make_shared<T>(sink);
}

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

#define EGGCPT_LOG(prefix, level, ...)                                        \
    eggcpt::default_sink()->sink(                                             \
        fmt::format(prefix" {}:{} :: {}\n", EGGCPT_FUNCTION, __LINE__,        \
            fmt::format(__VA_ARGS__)), eggcpt::Level::level)

#if EGGCPT_LOG_LEVEL <= 0
#  define EGGCPT_LOG_DEBUG(...) EGGCPT_LOG("[D]", Debug, __VA_ARGS__)
#else
#  define EGGCPT_LOG_DEBUG(...) static_cast<void>(0)
#endif

#if EGGCPT_LOG_LEVEL <= 1
#  define EGGCPT_LOG_INFO(...)  EGGCPT_LOG("[I]", Info,  __VA_ARGS__)
#else
#  define EGGCPT_LOG_INFO(...)  static_cast<void>(0)
#endif

#if EGGCPT_LOG_LEVEL <= 2
#  define EGGCPT_LOG_WARN(...)  EGGCPT_LOG("[W]", Warn,  __VA_ARGS__)
#else
#  define EGGCPT_LOG_WARN(...)  static_cast<void>(0)
#endif

#if EGGCPT_LOG_LEVEL <= 3
#  define EGGCPT_LOG_ERROR(...) EGGCPT_LOG("[E]", Error, __VA_ARGS__)
#else
#  define EGGCPT_LOG_ERROR(...) static_cast<void>(0)
#endif

#if EGGCPT_LOG_LEVEL <= 4
#  define EGGCPT_LOG_FATAL(...) EGGCPT_LOG("[F]", Fatal, __VA_ARGS__)
#else
#  define EGGCPT_LOG_FATAL(...) static_cast<void>(0)
#endif
