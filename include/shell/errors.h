#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

#include <shell/fmt.h>

namespace shell
{

class Error : public std::exception
{
public:
    Error(std::string_view message)
        : _message(message) {}

    template<typename... Args>
    Error(std::string_view format, Args&&... args)
        : _message(fmt::format(fmt::runtime(format), std::forward<Args>(args)...)) {}

    const char* what() const noexcept override
    {
        return _message.c_str();
    }

private:
    std::string _message;
};

class ParseError : public Error
{
public:
    using Error::Error;
};

}  // namespace shell
