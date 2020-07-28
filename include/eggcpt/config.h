#pragma once

#include <algorithm>
#include <fstream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

#include "filesystem.h"
#include "iterator.h"

namespace eggcpt
{

template<typename T>
class config
{
public:
    using string = std::basic_string<T>;

    bool parse(const filesystem::path& path)
    {
        std::basic_ifstream<T> stream(filesystem::make_absolute(path));
        if (!stream.is_open())
            return false;

        string line;
        while (std::getline(stream, line))
        {
            trim(line);
            
            if (line.empty())
                continue;

            switch (line.front())
            {
            case kCharSectionBegin:
                if (!parse_section(line))
                    return false;
                break;

            case kCharComment:
                if (!parse_comment(line))
                    return false;
                break;

            default:
                if (!parse_property(line))
                    return false;
                break;
            }
        }
        return true;
    }

private:
    static constexpr T kCharSectionBegin = '[';
    static constexpr T kCharSectionEnd   = ']';
    static constexpr T kCharComment      = '#';
    static constexpr T kCharAssign       = '=';
    static constexpr T kCharSpace        = ' ';
    static constexpr T kCharVectorBegin  = '[';
    static constexpr T kCharVectorEnd    = ']';

    string& trim(string& str)
    {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
            return !std::isspace(ch);
        }));

        str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), str.end());

        return str;
    }

    bool parse_section(const string& line)
    {
        auto size = 0;
        auto iter = line.begin();

        for (; iter != line.end(); ++iter)
        {
            if (*iter == kCharSpace) return false;
            if (*iter == kCharSectionBegin) continue;
            if (*iter == kCharSectionEnd) break;

            ++size;
        }

        if (size == 0 || iter == line.end())
            return false;

        tokens.push_back(trim(line.substr(1, size)));

        return true;
    }

    bool parse_comment(const string& line)
    {
        return true;
    }

    bool parse_property(const string& line)
    {
        return true;
    }

    std::vector<std::basic_string<T>> tokens;
};

}  // namespace eggcpt