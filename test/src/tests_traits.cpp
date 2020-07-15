#include "tests.h"

#include <eggcpt/traits.h>

using namespace eggcpt;

void Tests::addTestsTraits()
{
    add("is_integer", []() {
        bool result = true;
        result &= traits::is_integer_v<int>;
        result &= !traits::is_integer_v<float>;
        result &= !traits::is_integer_v<double>;

        return result;
    });

    add("is_any_of", []() {
        bool result = true;
        result &= traits::is_any_of_v<int, int, float>;
        result &= !traits::is_any_of_v<int, float>;
        result &= !traits::is_any_of_v<int, bool>;

        return result;
    });

    add("is_size_min", []() {
        bool result = true;
        result &= traits::is_size_min_v<int, 2>;
        result &= traits::is_size_min_v<int, 4>;
        result &= !traits::is_size_min_v<int, 8>;

        return result;
    });

    add("is_size_max", []() {
        bool result = true;
        result &= traits::is_size_max_v<int, 8>;
        result &= traits::is_size_max_v<int, 4>;
        result &= !traits::is_size_max_v<int, 2>;

        return result;
    });
}
