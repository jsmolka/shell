#pragma once

#include <cstddef>
#include <eggcpt/macro.h>

#ifdef __cpp_lib_filesystem
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

#if EGGCPT_PLATFORM_WINDOWS
#include <eggcpt/windows.h>
#elif EGGCPT_PLATFORM_LINUX
#include <unistd.h>
#elif EGGCPT_PLATFORM_DARWIN
#include <sys/syslimits.h>
#endif

namespace eggcpt::filesystem
{

#ifdef __cpp_lib_filesystem
using namespace std::filesystem;
#else
using namespace std::experimental::filesystem;
#endif

inline path executable()
{
    #if EGGCPT_PLATFORM_WINDOWS
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, sizeof(buffer) / sizeof(wchar_t));
    return buffer;
    #endif
    
    #if EGGCPT_PLATFORM_LINUX
    char buffer[PATH_MAX];
    readlink("/proc/self/exe", buffer, sizeof(buffer));
    return buffer;
    #endif

    #if EGGCPT_PLATFORM_DARWIN
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    _NSGetExecutablePath(buffer, &size);
    return buffer;
    #endif
}

inline path executable_dir()
{
    return executable().parent_path();
}

inline path make_absolute(const path& path)
{
    return path.is_relative()
        ? executable_dir() / path
        : path;
}

}  // namespace eggcpt::filesystem
