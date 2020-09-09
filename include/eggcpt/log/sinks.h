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

class SinkInterface
{
public:
    virtual ~SinkInterface() = default;

    virtual void sink(const std::string& message, Level level) = 0;
};

class ConsoleSink : public SinkInterface
{
public:
    void sink(const std::string& message, Level) override
    {
        fmt::print(message);
    }
};

class ColoredConsoleSink : public SinkInterface
{
public:
    ColoredConsoleSink()
    {
        #if EGGCPT_OS_WINDOWS
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (handle != INVALID_HANDLE_VALUE)
        {
            DWORD mode;
            if (GetConsoleMode(handle, &mode))
                SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
        #endif
    }

    void sink(const std::string& message, Level level) override
    {
        switch (level)
        {
        case Level::Debug: fmt::print(fmt::fg(fmt::rgb( 97, 214, 214)), message); break;
        case Level::Warn:  fmt::print(fmt::fg(fmt::rgb(249, 241, 165)), message); break;
        case Level::Error: fmt::print(fmt::fg(fmt::rgb(231,  72,  86)), message); break;
        case Level::Fatal: fmt::print(fmt::fg(fmt::rgb(180,   0, 158)), message); break;
        default:           fmt::print(                                  message); break;
        }
    }
};

class FileSink : public SinkInterface
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

template<typename... Sinks>
class MultiSink : public SinkInterface
{
    static_assert(sizeof...(Sinks) > 0);

public:
    MultiSink(Sinks&&... sinks)
        : sinks({ std::make_shared<Sinks>(std::move(sinks))... }) {}

    void sink(const std::string& message, Level level) override
    {
        for (auto& sink : sinks)
            sink->sink(message, level);
    }

private:
    std::array<std::shared_ptr<SinkInterface>, sizeof...(Sinks)> sinks;
};

namespace detail
{

inline std::shared_ptr<SinkInterface> default_sink = std::make_shared<ColoredConsoleSink>();

}  // namespace detail

template<typename Sink>
void setSink(Sink&& sink)
{
    static_assert(std::is_base_of_v<SinkInterface, Sink>);

    detail::default_sink = std::make_shared<Sink>(std::move(sink));
}

}  // namespace eggcpt

#define EGGCPT_LOG(prefix, level, ...)                                  \
    eggcpt::detail::default_sink->sink(                                 \
        fmt::format(prefix" {}:{} :: {}\n", EGGCPT_FUNCTION, __LINE__,  \
            fmt::format(__VA_ARGS__)), level)
