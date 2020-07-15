#pragma once

#include <functional>
#include <string>
#include <vector>

class Tests
{
public:
    Tests();

    int run() const;

private:
    struct Test
    {
        std::string name;
        std::function<bool(void)> func;
    };

    void add(const std::string& name, const std::function<bool(void)>& func);
    void addTestsBits();

    std::vector<Test> tests;
};
