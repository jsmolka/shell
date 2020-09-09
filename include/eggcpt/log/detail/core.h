#pragma once

#include <array>
#include <memory>
#include <type_traits>

#include <eggcpt/filesystem.h>
#include <eggcpt/fmt.h>
#include <eggcpt/int.h>
#include <eggcpt/windows.h>

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
    void sink(const std::string& message, Level) override
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

    void sink(const std::string& message, Level level) override
    {
        fmt::rgb color;
        switch (level)
        {
        case Level::Debug: color = fmt::rgb( 97, 214, 214); break;
        case Level::Info:  color = fmt::rgb(204, 204, 204); break;
        case Level::Warn:  color = fmt::rgb(249, 241, 165); break;
        case Level::Error: color = fmt::rgb(231,  72,  86); break;
        case Level::Fatal: color = fmt::rgb(180,   0, 158); break;
        }
        fmt::print(fmt::fg(color), message);
    }
};

class FileSink : public Sink
{
public:
    FileSink(const filesystem::path& file, bool trunc = false)
        : stream(filesystem::makeAbsolute(file),
            trunc ? std::ios::trunc : std::ios::app) {}

    void sink(const std::string& message, Level) override
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
        : sinks({ std::make_shared<Ts>(std::move(sinks))... }) {}

    void sink(const std::string& message, Level level) override
    {
        for (const auto& sink : sinks)
            sink->sink(message, level);
    }

private:
    std::array<std::shared_ptr<Sink>, sizeof...(Ts)> sinks;
};

namespace detail
{

inline std::shared_ptr<Sink> sink = std::make_shared<ColoredConsoleSink>();

}  // namespace detail

template<typename T>
void setSink(const T& sink)
{
    static_assert(std::is_base_of_v<Sink, T>);

    detail::sink = std::make_shared<T>(sink);
}

}  // namespace eggcpt

#define EGGCPT_LOG(prefix, level, ...)                                  \
    eggcpt::detail::sink->sink(                                         \
        fmt::format(prefix" {}:{} :: {}\n", EGGCPT_FUNCTION, __LINE__,  \
            fmt::format(__VA_ARGS__)), level)
