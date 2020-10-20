#pragma once

#include <array>
#include <memory>
#include <type_traits>

#include <shell/filesystem.h>
#include <shell/fmt/color.h>
#include <shell/fmt/format.h>
#include <shell/int.h>
#include <shell/macros.h>
#include <shell/windows.h>

namespace shell
{

namespace log
{

enum class Level { Debug, Info, Warn, Error, Fatal };

class BasicSink
{
public:
    using Pointer = std::shared_ptr<BasicSink>;

    virtual ~BasicSink() = default;

    virtual void sink(const std::string& location, const std::string& message, Level level) = 0;
};

class ConsoleSink : public BasicSink
{
public:
    void sink(const std::string& location, const std::string& message, Level) override
    {
        fmt::print("{}: {}\n", location, message);
    }
};

class ColoredConsoleSink : public BasicSink
{
public:
    ColoredConsoleSink()
    {
        #if SHELL_OS_WINDOWS
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
        fmt::print(style(level), "{}: {}\n", location, message);
    }

private:
    static fmt::text_style style(Level level)
    {
        switch (level)
        {
        case Level::Debug: return fmt::fg(fmt::rgb( 97, 214, 214));
        case Level::Warn:  return fmt::fg(fmt::rgb(249, 241, 165));
        case Level::Error: return fmt::fg(fmt::rgb(231,  72,  86));
        case Level::Fatal: return fmt::fg(fmt::rgb(180,   0, 158));
        }
        return fmt::text_style();
    }
};

class FileSink : public BasicSink
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
class MultiSink : public BasicSink
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
    std::array<BasicSink::Pointer, sizeof...(Sinks)> sinks;
};

inline BasicSink::Pointer default_sink = std::make_shared<ColoredConsoleSink>();

}  // namespace detail

template<typename Sink, typename... Sinks>
void setSink(Sink&& sink, Sinks&&... sinks)
{
    static_assert(std::is_base_of_v<BasicSink, Sink>);
    static_assert(std::conjunction_v<std::is_base_of<BasicSink, Sinks>...>);

    if constexpr (sizeof...(Sinks) == 0)
        detail::default_sink = std::make_shared<Sink>(std::move(sink));
    else 
        detail::default_sink = std::make_shared<
            detail::MultiSink<Sink, Sinks...>>(
                std::move(sink), std::move(sinks)...);
}

}  // namespace log

using log::BasicSink;
using log::ConsoleSink;
using log::ColoredConsoleSink;
using log::FileSink;
using log::Level;
using log::setSink;

}  // namespace shell

#define SHELL_LOG(prefix, level, ...)                  \
    shell::log::detail::default_sink->sink(            \
        fmt::format("{} {}", prefix, SHELL_FUNCTION),  \
        fmt::format(__VA_ARGS__),                      \
        level)
