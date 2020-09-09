#pragma once

#include <stdexcept>
#include <string>

#include <eggcpt/fmt.h>

namespace eggcpt
{

class Error : public std::exception
{
public:
    Error(const std::string& message)
        : message(message) {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;
};

class FormattedError : public Error
{
public:
    template<typename... Args>
    FormattedError(const std::string& format, Args&&... args)
        : Error(fmt::format(format, std::forward<Args>(args)...)) {}
};

}  // namespace eggcpt
