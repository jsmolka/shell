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
        : message(message) {}

    template<typename... Args>
    Error(const std::string& format, Args&&... args)
        : message(fmt::format(format, std::forward<Args>(args)...)) {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;
};

class ParseError : public Error
{
public:
    using Error::Error;
};

}  // namespace shell
