#pragma once

namespace eggcpt
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

private:
    const_iterator begin_{};
    const_iterator end_{};
};

template<typename T>
auto make_iterator_range(T begin, T end)
{
    return iterator_range<T>(begin, end);
}

template<typename T>
auto reversed(T& range)
{
    return make_iterator_range(std::rbegin(range), std::rend(range));
}

}  // namespace eggcpt
