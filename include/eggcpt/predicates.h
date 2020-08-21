#pragma once

#include <functional>

namespace eggcpt
{

namespace detail
{

template<typename Value, typename Comparator>
class Predicate
{
public:
    explicit Predicate(const Value& other)
        : other(other) {}

    bool operator()(const Value& value) const
    {
        return Comparator()(value, other);
    }

private:
    const Value& other;
};

}  // namespace detail

template<typename Value> using PredicateEq = detail::Predicate<Value, std::equal_to<Value>>;
template<typename Value> using PredicateNe = detail::Predicate<Value, std::not_equal_to<Value>>;
template<typename Value> using PredicateLe = detail::Predicate<Value, std::less_equal<Value>>;
template<typename Value> using PredicateLt = detail::Predicate<Value, std::less<Value>>;
template<typename Value> using PredicateGe = detail::Predicate<Value, std::greater_equal<Value>>;
template<typename Value> using PredicateGt = detail::Predicate<Value, std::greater<Value>>;

}  // namespace eggcpt
