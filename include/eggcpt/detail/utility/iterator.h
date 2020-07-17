#pragma once

namespace eggcpt::utility
{

template<typename T>
class iterator_range
{
public:
    using iterator = T;
    using const_iterator = const T;

    iterator_range() = default;
    iterator_range(T begin, T end)
        : begin_(begin), end_(end) {}

    iterator begin() { return begin_; }
    iterator end() { return end_; }
    const_iterator cbegin() const { return begin_; }
    const_iterator cend() const { return end_; }

    bool empty() const { return cbegin() == cend(); }

private:
    const_iterator begin_{};
    const_iterator end_{};
};

}  // namespace eggcpt::utility
