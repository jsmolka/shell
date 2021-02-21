#pragma once

#include <stdexcept>
#include <string>

#include <shell/fmt.h>

namespace shell
{

class Error : public std::exception
{
public:
    Error(const std::string& message)
        : _message(message) {}

    template<typename... Args>
    Error(const std::string& format, Args&&... args)
        : _message(fmt::format(format, std::forward<Args>(args)...)) {}

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
