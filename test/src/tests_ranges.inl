TEST_CASE("ranges::ForwardRange")
{
    int expected = 1;
    
    std::vector<int> values = { 1, 2, 3 };
    for (auto& value : ForwardRange(values.begin(), values.end()))
        REQUIRE(value == expected++);

    expected = 1;

    const std::vector<int> const_values = { 1, 2, 3 };
    for (const auto& value : ForwardRange(const_values.begin(), const_values.end()))
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
    int x = -1;
    for (auto y : range(10))
        REQUIRE(y == ++x);
    REQUIRE(x == 9);

    x = 4;
    for (auto y : range(5, 10))
        REQUIRE(y == ++x);
    REQUIRE(x == 9);

    x = 3;
    for (auto y : range(5, 10, 2))
    {
        x += 2;
        REQUIRE(y == x);
    }
    REQUIRE(x == 9);

    x = 7;
    for (auto y : range(5, -5, -2))
    {
        x -= 2;
        REQUIRE(y == x);
    }
    REQUIRE(x == -5);
}

TEST_CASE("ranges::enumerate")
{
    std::vector<int> x1 = { 0, 1, 2, 3, 4 };
    std::vector<int> y1 = { 1, 2, 3, 4, 5 };

    for (auto [index, value] : enumerate(x1))
    {
        REQUIRE(value == index);
        REQUIRE(value == x1[index]);
        value++;
    }
    REQUIRE(x1 == y1);

    int x2[5] = { 1, 2, 3, 4, 5 };
    int y2[5] = { 2, 3, 4, 5, 6 };

    for (auto [index, value] : enumerate(x2, 1))
    {
        REQUIRE(value == index);
        REQUIRE(value == x2[index - 1]);
        value++;
    }
    REQUIRE(std::memcmp(x2, y2, sizeof(x2)) == 0);
}

TEST_CASE("ranges::zip")
{
    int t1[4]                       = { 0, 1, 2, 3 };
    std::vector<int> t2             = { 1, 2, 3, 4 };
    std::array<int, 4> t3           = { 2, 3, 4, 5 };
    constexpr int t4[4]             = { 3, 4, 5, 6 };
    const std::vector<int> t5       = { 4, 5, 6, 7 };
    constexpr std::array<int, 4> t6 = { 5, 6, 7, 8 };

    int x = 0;
    for (auto [i, j, k, l, m, n] : zip(t1, t2, t3, t4, t5, t6))
    {
        REQUIRE(i == x + 0);
        REQUIRE(j == x + 1);
        REQUIRE(k == x + 2);
        REQUIRE(l == x + 3);
        REQUIRE(m == x + 4);
        REQUIRE(n == x + 5);
        x++;
    }
    REQUIRE(x == 4);
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
