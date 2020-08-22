#pragma once

#include <stdexcept>

#include <eggcpt/fmt.h>

namespace eggcpt
{

class FormattedError : public std::exception
{
public:
    template<typename String, typename... Args>
    FormattedError(const String& format, Args&&... args)
        : message(fmt::format(format, std::forward<Args>(args)...)) {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;
};

}  // namespace eggcpt
