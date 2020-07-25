#pragma once

#include <cstddef>
#include <fstream>
#ifdef __cpp_lib_filesystem
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

#include <eggcpt/traits.h>
#include <eggcpt/windows.h>

#if EGGCPT_OS_LINUX
#include <unistd.h>
#elif EGGCPT_OS_DARWIN
#include <mach-o/dyld.h>
#endif

namespace eggcpt::filesystem
{

namespace detail
{

template<typename T> using has_data_t   = decltype(std::declval<T>().data());
template<typename T> using has_size_t   = decltype(std::declval<T>().size());
template<typename T> using has_resize_t = decltype(std::declval<T>().resize(0));

}  // namespace detail

#ifdef __cpp_lib_filesystem
using namespace std::filesystem;
#else
using namespace std::experimental::filesystem;
#endif

inline path executable_path()
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

inline path make_absolute(const path& path)
{
    return path.is_relative()
        ? executable_path() / path
        : path;
}

template<typename T, std::enable_if_t<is_detected_v<T, detail::has_resize_t>, int> = 0>
bool read(const path& file, T& dst)
{
    using value_type = typename T::value_type;

    static_assert(is_detected_v<T, detail::has_data_t>);
    static_assert(is_detected_v<T, detail::has_size_t>);

    auto stream = std::ifstream(file, std::ios::binary);
    if (!stream.is_open())
        return false;

    stream.seekg(0, std::ios::end);
    std::size_t size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    dst.resize((size + sizeof(value_type) - 1) / sizeof(value_type));

    stream.read(reinterpret_cast<char*>(dst.data()), size);

    return true;
}

template<typename T, std::enable_if_t<!is_detected_v<T, detail::has_resize_t>, int> = 0>
bool read(const path& file, T& dst)
{
    using value_type = typename T::value_type;

    static_assert(is_detected_v<T, detail::has_data_t>);
    static_assert(is_detected_v<T, detail::has_size_t>);

    auto stream = std::ifstream(file, std::ios::binary);
    if (!stream.is_open())
        return false;

    stream.seekg(0, std::ios::end);
    std::size_t size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    if (size != (dst.size() * sizeof(value_type)))
        return false;

    stream.read(reinterpret_cast<char*>(dst.data()), size);

    return true;
}

template<typename T>
bool write(const path& file, const T& src)
{
    using value_type = typename T::value_type;

    static_assert(is_detected_v<T, detail::has_data_t>);
    static_assert(is_detected_v<T, detail::has_size_t>);

    auto stream = std::ofstream(file, std::ios::binary);
    if (!stream.is_open())
        return false;

    stream.write(reinterpret_cast<const char*>(src.data()), src.size() * sizeof(value_type));

    return true;
}

}  // namespace eggcpt::filesystem
