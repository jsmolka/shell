TEST_CASE("reconstruct")
{
    struct Test1
    {
        int x = 0;
        int y = 0;
    };

    Test1 test1{1, 1};
    REQUIRE(test1.x == 1);
    REQUIRE(test1.y == 1);

    reconstruct(test1);
    REQUIRE(test1.x == 0);
    REQUIRE(test1.y == 0);

    struct Test2
    {
        Test2(int x, int y) : x(x), y(y) {}

        int x = 0;
        int y = 0;
    };

    Test2 test2(1, 1);
    REQUIRE(test2.x == 1);
    REQUIRE(test2.y == 1);

    reconstruct(test2, 0, 0);
    REQUIRE(test2.x == 0);
    REQUIRE(test2.y == 0);
}

TEST_CASE("enumerate")
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
