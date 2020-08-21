#pragma once

#include <cstring>
#include <codecvt>
#include <vector>

#include <eggcpt/macros.h>

#if EGGCPT_OS_WINDOWS

int main(int argc, char* argv[]);

#pragma comment(linker, "/ENTRY:wmainCRTStartup")

int wmain(int argc, wchar_t* argv[])
{
    static_assert(sizeof(wchar_t) == sizeof(char16_t));

    std::vector<char*> args;
    args.reserve(argc);

    for (int x = 0; x < argc; ++x)
    {
        auto cvt = std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t>();
        auto utf = cvt.to_bytes(reinterpret_cast<char16_t*>(argv[x]));

        args.push_back(strdup(utf.c_str()));
    }

    int value = main(argc, args.data());

    for (char* arg : args)
        free(arg);

    return value;
}

#endif
