#pragma once

#include <array>
#include <memory>
#include <type_traits>

#include <shell/filesystem.h>
#include <shell/fmt.h>
#include <shell/macros.h>
#include <shell/windows.h>

namespace shell
{

enum class Level { Debug, Info, Warn, Error, Fatal };

class BasicSink
{
public:
    using Pointer = std::shared_ptr<BasicSink>;

    virtual ~BasicSink() = default;

    virtual void sink(const std::string& message, Level level) = 0;
    virtual void sink(const std::string& message, Level level, const std::string& location) = 0;

protected:
    static std::string_view prefix(Level level)
    {
        switch (level)
        {
        case Level::Debug: return "[D]";
        case Level::Info:  return "[I]";
        case Level::Warn:  return "[W]";
        case Level::Error: return "[E]";
        case Level::Fatal: return "[F]";

        default:
            SHELL_UNREACHABLE;
            return "[?]";
        }
    }
};

class ConsoleSink : public BasicSink
{
public:
    void sink(const std::string& message, Level level)
    {
        fmt::print("{} {}\n", prefix(level), message);
    }

    void sink(const std::string& message, Level level, const std::string& location)
    {
        fmt::print("{} {}: {}\n", prefix(level), location, message);
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

    void sink(const std::string& message, Level level)
    {
        fmt::print(style(level), "{} {}\n", prefix(level), message);
    }

    void sink(const std::string& message, Level level, const std::string& location)
    {
        fmt::print(style(level), "{} {}: {}\n", prefix(level), location, message);
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
    FileSink(const filesystem::path& file, std::ios::openmode mode = std::ios::trunc)
        : _stream(file, mode) {}

    void sink(const std::string& message, Level level)
    {
        if (_stream && _stream.is_open())
            _stream << fmt::format("{} {}\n", prefix(level), message);
    }

    void sink(const std::string& message, Level level, const std::string& location)
    {
        if (_stream && _stream.is_open())
            _stream << fmt::format("{} {}: {}\n", prefix(level), location, message);
    }

private:
    std::ofstream _stream;
};

template<typename... Sinks>
class MultiSink : public BasicSink
{
public:
    static_assert(sizeof...(Sinks) > 0);

    MultiSink(Sinks&&... sinks)
        : _sinks({ std::make_shared<Sinks>(std::move(sinks))... }) {}

    void sink(const std::string& message, Level level)
    {
        for (auto& sink : _sinks)
            sink->sink(message, level);
    }

    void sink( const std::string& message, Level level, const std::string& location)
    {
        for (auto& sink : _sinks)
            sink->sink(message, level, location);
    }

private:
    std::array<BasicSink::Pointer, sizeof...(Sinks)> _sinks;
};

namespace detail
{

inline BasicSink::Pointer sink = std::make_shared<ColoredConsoleSink>();

}  // namespace detail

template<typename Sink, typename... Sinks>
void setSink(Sink&& sink, Sinks&&... sinks)
{
    static_assert(std::is_base_of_v<BasicSink, Sink>);
    static_assert(std::conjunction_v<std::is_base_of<BasicSink, Sinks>...>);

    if constexpr (sizeof...(Sinks) == 0)
        detail::sink = std::make_shared<Sink>(std::move(sink));
    else 
        detail::sink = std::make_shared<MultiSink<Sink, Sinks...>>(std::move(sink), std::move(sinks)...);
}

template<typename... Args>
void debug(const std::string& format, Args&&... args)
{
    detail::sink->sink(fmt::format(format, std::forward<Args>(args)...), Level::Debug);
}

template<typename... Args>
void info(const std::string& format, Args&&... args)
{
    detail::sink->sink(fmt::format(format, std::forward<Args>(args)...), Level::Info);
}

template<typename... Args>
void warn(const std::string& format, Args&&... args)
{
    detail::sink->sink(fmt::format(format, std::forward<Args>(args)...), Level::Warn);
}

template<typename... Args>
void error(const std::string& format, Args&&... args)
{
    detail::sink->sink(fmt::format(format, std::forward<Args>(args)...), Level::Error);
}

template<typename... Args>
void fatal(const std::string& format, Args&&... args)
{
    detail::sink->sink(fmt::format(format, std::forward<Args>(args)...), Level::Fatal);
}

}  // namespace shell

#define SHELL_LOG(level, ...) shell::detail::sink->sink(fmt::format(__VA_ARGS__), level, SHELL_FUNCTION)
