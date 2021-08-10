#pragma once

#include <shell/predef.h>
#include <shell/ranges.h>

#if SHELL_CC_MSVC

#include <codecvt>
#include <cstring>
#include <vector>

int main(int argc, char* argv[]);

#pragma comment(linker, "/ENTRY:wmainCRTStartup")

int wmain(int argc, wchar_t* argv[])
{
    using Convert = std::wstring_convert<std::codecvt_utf8<wchar_t>>;

    std::vector<char*> args;
    args.reserve(argc);

    for (int i = 0; i < argc; ++i)
    {
        auto string = Convert().to_bytes(argv[i]);
        args.push_back(strdup(string.c_str()));
    }

    int ret = main(argc, args.data());

    shell::foreach(args, free);

    return ret;
}

#endif
