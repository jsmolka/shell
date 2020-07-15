#include "tests.h"

#include <iostream>

Tests::Tests()
{
    addTestsBits();
    addTestsTraits();
}

int Tests::run() const
{
    for (const auto& test : tests)
    {
        if (!test.func())
        {
            std::cout << test.name << std::endl;
            return 1;
        }
    }
    return 0;
}

void Tests::add(const std::string& name, const std::function<bool(void)>& func)
{
    tests.push_back({ name, func });
}
