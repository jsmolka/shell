#pragma once

namespace eggcpt
{

template<typename T>
class singleton
{
public:
    singleton(const singleton&) = delete;
    singleton& operator=(const singleton&) = delete;

    static T& instance()
    {
        static T instance{ token{} };
        return instance;
    }

protected:
    struct token {};

    singleton() = default;
    virtual ~singleton() = default;
};

}  // namespace eggcpt
