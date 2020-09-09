#pragma once

#include <fstream>
#ifdef __cpp_lib_filesystem
#  include <filesystem>
#else
#  include <experimental/filesystem>
#endif

#include <eggcpt/traits.h>
#include <eggcpt/windows.h>

#if EGGCPT_OS_LINUX
#  include <limits.h>
#  include <unistd.h>
#elif EGGCPT_OS_DARWIN
#  include <mach-o/dyld.h>
#endif

namespace eggcpt::filesystem
{

namespace detail
{

template<typename T>
using resize_t = decltype(std::declval<T>().resize(1));

template<typename T>
constexpr bool is_resizable_v = is_detected_v<T, resize_t>;

}  // namespace detail

#ifdef __cpp_lib_filesystem
using namespace std::filesystem;
#else
using namespace std::experimental::filesystem;
#endif

inline path executablePath()
{
    #if EGGCPT_OS_WINDOWS
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, sizeof(buffer) / sizeof(wchar_t));
    return path(buffer).parent_path();

    #elif EGGCPT_OS_LINUX
    char buffer[PATH_MAX];
    readlink("/proc/self/exe", buffer, sizeof(buffer));
    return path(buffer).parent_path();

    #elif EGGCPT_OS_DARWIN
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    _NSGetExecutablePath(buffer, &size);
    return path(buffer).parent_path();

    #else
    return current_path();
    #endif
}

inline path makeAbsolute(const path& path)
{
    return path.is_relative()
        ? executablePath() / path
        : path;
}

template<typename Container>
bool read(const path& file, Container& dst)
{
    static_assert(sizeof(typename Container::value_type) == 1);

    auto stream = std::ifstream(file, std::ios::binary);
    if (!stream.is_open())
        return false;

    stream.seekg(0, std::ios::end);
    std::size_t size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    if constexpr (detail::is_resizable_v<Container>)
        dst.resize(size);

    if (size != dst.size())
        return false;

    stream.read(reinterpret_cast<char*>(dst.data()), size);

    return true;
}

template<typename Container>
bool write(const path& file, const Container& src)
{
    static_assert(sizeof(typename Container::value_type) == 1);

    auto stream = std::ofstream(file, std::ios::binary);
    if (!stream.is_open())
        return false;

    stream.write(reinterpret_cast<const char*>(src.data()), src.size());

    return true;
}

}  // namespace eggcpt::filesystem
