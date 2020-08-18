#pragma once

namespace eggcpt
{

#define EGGCPT_PREDICATE(suffix, compare)       \
    template<typename Value>                    \
    auto predicate##suffix(const Value& other)  \
    {                                           \
        return [&](const Value& value) {        \
            return value compare other;         \
        };                                      \
    }

EGGCPT_PREDICATE(Eq, ==);
EGGCPT_PREDICATE(Ne, !=);
EGGCPT_PREDICATE(Le, <=);
EGGCPT_PREDICATE(Lt, < );
EGGCPT_PREDICATE(Ge, >=);
EGGCPT_PREDICATE(Gt, > );

#undef EGGCPT_PREDICATE

}  // namespace eggcpt
