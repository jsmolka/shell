TEST_CASE("ranges::IteratorRange")
{
    int expected = 1;
    
    std::vector<int> values = { 1, 2, 3 };
    for (auto& value : IteratorRange(values.begin(), values.end()))
        REQUIRE(value == expected++);

    expected = 1;

    const std::vector<int> const_values = { 1, 2, 3 };
    for (const auto& value : IteratorRange(const_values.begin(), const_values.end()))
        REQUIRE(value == expected++);
}

TEST_CASE("ranges::PointerRange")
{
    int expected = 1;

    u8 data[3] = { 1, 2, 3 };
    for (auto& value : PointerRange(data, 3))
        REQUIRE(value == expected++);

    expected = 1;

    const u8 const_data[3] = { 1, 2, 3 };
    for (const auto& value : PointerRange(const_data, 3))
        REQUIRE(value == expected++);
}

TEST_CASE("ranges::range")
{
    int x = 0;
    for (auto y : range(10))
        REQUIRE(y == x++);

    x = 5;
    for (auto y : range(5, 10))
        REQUIRE(y == x++);

    x = 5;
    for (auto y : range(5, 10, 2))
    {
        REQUIRE(y == x);
        x += 2;
    }
}

TEST_CASE("ranges::reversed")
{
    int expected = 3;

    std::vector<int> values = { 1, 2, 3 };
    for (auto& value : reversed(values))
        REQUIRE(value == expected--);

    expected = 3;

    const std::vector<int> const_values = { 1, 2, 3 };
    for (const auto& value : reversed(const_values))
        REQUIRE(value == expected--);
}

TEST_CASE("iterator")
{
    std::vector<int> values = { 1, 2, 3 };
    int expected = values.back();

    for (const auto& value : reversed(values))
        REQUIRE(value == expected--);

    uint x = 0;
    for (auto& v : PointerRange(reinterpret_cast<u8*>(&x), 4))
    {
        v++;
    }
    REQUIRE(x == 0x01010101);
}
