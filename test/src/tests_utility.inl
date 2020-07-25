TEST_CASE("utility::reconstruct")
{
    struct Test
    {
        int x{};
        int y{};
    };
        
    Test test{ 1, 1 };
    reconstruct(&test);

    REQUIRE(test.x == 0);
    REQUIRE(test.y == 0);
}

TEST_CASE("utility::reconstruct<Args>")
{
    struct Test
    {
        Test(int x, int y)
            : x(x), y(y) {}

        int x{};
        int y{};
    };

    Test test(1, 1);
    reconstruct(&test, 0, 0);

    REQUIRE(test.x == 0);
    REQUIRE(test.y == 0);
}
