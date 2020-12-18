#pragma once

#include <fstream>

#include <shell/fmt.h>
#include <shell/parse.h>
#include <shell/predef.h>
#include <shell/traits.h>

#ifdef __cpp_lib_filesystem
#  include <filesystem>
#  define SHELL_FILESYSTEM_NAMESPACE std::filesystem
#  define SHELL_FILESYSTEM_EXPERIMENTAL 0
#else
#  include <experimental/filesystem>
#  define SHELL_FILESYSTEM_NAMESPACE std::experimental::filesystem
#  define SHELL_FILESYSTEM_EXPERIMENTAL 1
#endif

namespace shell::filesystem
{

using namespace SHELL_FILESYSTEM_NAMESPACE;

namespace detail
{

template<typename T>
using resize_t = decltype(std::declval<T>().resize(1));

template<typename T>
inline constexpr bool is_resizable_v = is_detected_v<T, resize_t>;

}  // namespace detail

enum class Status { Ok, BadFile, BadStream, BadSize };

template<typename Container>
Status read(const path& file, Container& dst)
{
    static_assert(sizeof(typename Container::value_type) == 1);

    std::ifstream stream(file, std::ios::binary);

    if (!stream.is_open())
        return Status::BadFile;

    if (!stream)
        return Status::BadStream;

    std::size_t size = file_size(file);

    if constexpr (detail::is_resizable_v<Container>)
        dst.resize(size);

    if (dst.size() != size)
        return Status::BadSize;

    stream.read(reinterpret_cast<char*>(dst.data()), size);

    return Status::Ok;
}

template<typename Container>
Status write(const path& file, const Container& src)
{
    static_assert(sizeof(typename Container::value_type) == 1);

    std::ofstream stream(file, std::ios::binary);

    if (!stream.is_open())
        return Status::BadFile;

    if (!stream)
        return Status::BadStream;

    stream.write(reinterpret_cast<const char*>(src.data()), src.size());

    return Status::Ok;
}

}  // namespace shell::filesystem

#if SHELL_CC_EMSCRIPTEN

namespace shell::filesystem
{

inline path program()
{
    return current_path() / "main.js";
}

}  // namespace shell::filesystem

#elif SHELL_OS_WINDOWS

#include <shell/windows.h>

namespace shell::filesystem
{

inline path program()
{
    WCHAR path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);

    return filesystem::path(path);
}

}  // namespace shell::filesystem

#elif SHELL_OS_MACOS

#include <mach-o/dyld.h>

namespace shell::filesystem
{

inline path program()
{
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
        return filesystem::path(path);

    char* buffer = new char[size];
    _NSGetExecutablePath(buffer, &size);

    filesystem::path result(buffer);
    delete[] buffer;

    return result;
}

}  // namespace shell::filesystem

#elif SHELL_OS_BSD_FREE

#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdlib.h>

namespace shell::filesystem
{

inline path program()
{
    int mib[4];
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = -1;
    char buffer[10240];
    size_t cb = sizeof(buffer);
    sysctl(mib, 4, buffer, &cb, NULL, 0);

    return path(buffer);
}

}  // namespace shell::filesystem

#elif SHELL_OS_BSD_NET

namespace shell::filesystem
{

inline path program()
{
    return read_symlink("/proc/curproc/exe");
}

}  // namespace shell::filesystem

#elif SHELL_OS_BSD_DRAGONFLY

namespace shell::filesystem
{

inline path program()
{
    return read_symlink("/proc/curproc/file");
}

}  // namespace shell::filesystem

#else

namespace shell::filesystem
{

inline path program()
{
    return read_symlink("/proc/self/exe");
}

}  // namespace shell::filesystem

#endif

namespace shell::filesystem
{

inline path absolute(const path& path)
{
    return path.is_relative()
        ? program().parent_path() / path
        : path;
}

inline path absolute(const path& path, std::error_code&)
{
    return filesystem::absolute(path);
}

}  // namespace shell::filesystem

template<>
struct fmt::formatter<shell::filesystem::path>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const shell::filesystem::path& path, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), path.string());
    }
};

template<>
inline std::optional<shell::filesystem::path> shell::parse(const std::string& data)
{
    return shell::filesystem::u8path(data);
}
