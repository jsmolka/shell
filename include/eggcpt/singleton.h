#pragma once

namespace eggcpt
{

template<typename T>
class Singleton
{
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T& instance()
    {
        static T instance(Token{});
        return instance;
    }

protected:
    struct Token {};

    Singleton() = default;
    virtual ~Singleton() = default;
};

}  // namespace eggcpt
