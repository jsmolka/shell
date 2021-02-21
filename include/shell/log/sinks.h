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
    virtual void sink(const std::string& location, const std::string& message, Level level) = 0;

protected:
    static std::string format(const std::string& message, Level level)
    {
        return fmt::format("{} {}\n", prefix(level), message);
    }

    static std::string format(const std::string& location, const std::string& message, Level level)
    {
        return fmt::format("{} {}: {}\n", prefix(level), location, message);
    }

private:
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
        fmt::print(format(message, level));
    }

    void sink(const std::string& location, const std::string& message, Level level)
    {
        fmt::print(format(location, message, level));
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
        fmt::print(style(level), format(message, level));
    }

    void sink(const std::string& location, const std::string& message, Level level)
    {
        fmt::print(style(level), format(location, message, level));
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
            _stream << format(message, level);
    }

    void sink(const std::string& location, const std::string& message, Level level)
    {
        if (_stream && _stream.is_open())
            _stream << format(location, message, level);
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

    void sink(const std::string& location, const std::string& message, Level level)
    {
        for (auto& sink : _sinks)
            sink->sink(location, message, level);
    }

private:
    std::array<BasicSink::Pointer, sizeof...(Sinks)> _sinks;
};

namespace detail
{

inline BasicSink::Pointer default_sink = std::make_shared<ColoredConsoleSink>();

}  // namespace detail

template<typename Sink, typename... Sinks>
void setSink(Sink&& sink, Sinks&&... sinks)
{
    static_assert(std::is_base_of_v<BasicSink, Sink>);
    static_assert(std::conjunction_v<std::is_base_of<BasicSink, Sinks>...>);

    if constexpr (sizeof...(Sinks) == 0)
        detail::default_sink = std::make_shared<Sink>(
            std::move(sink));
    else 
        detail::default_sink = std::make_shared<MultiSink<Sink, Sinks...>>(
            std::move(sink), std::move(sinks)...);
}

template<typename String, typename... Args>
void debug(const String& format, Args&&... args)
{
    detail::default_sink->sink(fmt::format(format, std::forward<Args>(args)...), Level::Debug);
}

template<typename String, typename... Args>
void info(const String& format, Args&&... args)
{
    detail::default_sink->sink(fmt::format(format, std::forward<Args>(args)...), Level::Info);
}

template<typename String, typename... Args>
void warn(const String& format, Args&&... args)
{
    detail::default_sink->sink(fmt::format(format, std::forward<Args>(args)...), Level::Warn);
}

template<typename String, typename... Args>
void error(const String& format, Args&&... args)
{
    detail::default_sink->sink(fmt::format(format, std::forward<Args>(args)...), Level::Error);
}

template<typename String, typename... Args>
void fatal(const String& format, Args&&... args)
{
    detail::default_sink->sink(fmt::format(format, std::forward<Args>(args)...), Level::Fatal);
}

#define SHELL_LOG(level, ...)           \
    shell::detail::default_sink->sink(  \
        SHELL_FUNCTION,                 \
        fmt::format(__VA_ARGS__),       \
        level)

}  // namespace shell
