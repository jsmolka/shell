#pragma once

#include <shell/predef.h>
#include <shell/ranges.h>

#if SHELL_OS_WINDOWS

#include <algorithm>
#include <codecvt>
#include <cstring>
#include <vector>

int main(int argc, char* argv[]);

#pragma comment(linker, "/ENTRY:wmainCRTStartup")

int wmain(int argc, wchar_t* argv[])
{
    static_assert(sizeof(wchar_t) == sizeof(char16_t));

    std::vector<char*> args;
    args.reserve(argc);

    for (const auto& arg : shell::PointerRange(argv, argc))
    {
        auto converter = std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t>();
        auto converted = converter.to_bytes(reinterpret_cast<char16_t*>(arg));

        args.push_back(strdup(converted.c_str()));
    }

    int ret = main(argc, args.data());

    shell::foreach(args, free);

    return ret;
}

#endif
