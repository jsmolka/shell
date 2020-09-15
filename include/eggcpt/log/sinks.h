#pragma once

#include <array>
#include <memory>
#include <type_traits>

#include <eggcpt/filesystem.h>
#include <eggcpt/fmt.h>
#include <eggcpt/int.h>
#include <eggcpt/macros.h>
#include <eggcpt/windows.h>

namespace eggcpt
{

enum class Level { Debug, Info, Warn, Error, Fatal };

class SinkInterface
{
public:
    using Shared = std::shared_ptr<SinkInterface>;

    virtual ~SinkInterface() = default;

    virtual void sink(const std::string& location, const std::string& message, Level level) = 0;
};

class ConsoleSink : public SinkInterface
{
public:
    void sink(const std::string& location, const std::string& message, Level) override
    {
        fmt::print("{}: {}\n", location, message);
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

    void sink(const std::string& location, const std::string& message, Level level) override
    {
        fmt::text_style style;
        switch (level)
        {
        case Level::Debug: style = fmt::fg(fmt::rgb( 97, 214, 214)); break;
        case Level::Warn:  style = fmt::fg(fmt::rgb(249, 241, 165)); break;
        case Level::Error: style = fmt::fg(fmt::rgb(231,  72,  86)); break;
        case Level::Fatal: style = fmt::fg(fmt::rgb(180,   0, 158)); break;
        }
        fmt::print(style, "{}: {}\n", location, message);
    }
};

class FileSink : public SinkInterface
{
public:
    FileSink(const filesystem::path& file, bool trunc = false)
        : stream(filesystem::makeAbsolute(file),
            trunc ? std::ios::trunc : std::ios::app) {}

    void sink(const std::string& location, const std::string& message, Level) override
    {
        if (stream.is_open())
            stream << location << ": " << message << "\n";
    }

private:
    std::ofstream stream;
};

namespace detail
{

template<typename... Sinks>
class MultiSink : public SinkInterface
{
    static_assert(sizeof...(Sinks) > 0);

public:
    MultiSink(Sinks&&... sinks)
        : sinks({ std::make_shared<Sinks>(std::move(sinks))... }) {}

    void sink(const std::string& location, const std::string& message, Level level) override
    {
        for (auto& sink : sinks)
            sink->sink(location, message, level);
    }

private:
    std::array<SinkInterface::Shared, sizeof...(Sinks)> sinks;
};

inline SinkInterface::Shared default_sink = std::make_shared<ColoredConsoleSink>();

}  // namespace detail

template<typename Sink, typename... Sinks>
void setSink(Sink&& sink, Sinks&&... sinks)
{
    static_assert(std::is_base_of_v<SinkInterface, Sink>);
    static_assert(std::conjunction_v<std::is_base_of<SinkInterface, Sinks>...>);

    if constexpr (sizeof...(Sinks) == 0)
        detail::default_sink = std::make_shared<Sink>(std::move(sink));
    else 
        detail::default_sink = std::make_shared<
            detail::MultiSink<Sink, Sinks...>>(
                std::move(sink), std::move(sinks)...);
}

}  // namespace eggcpt

#define EGGCPT_LOG(prefix, level, ...)                  \
    eggcpt::detail::default_sink->sink(                 \
        fmt::format("{} {}", prefix, EGGCPT_FUNCTION),  \
        fmt::format(__VA_ARGS__),                       \
        level)
