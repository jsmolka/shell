namespace tests_utility
{

TEST_CASE("utility::reconstruct")
{
    struct test
    {
        int x{};
        int y{};
    };

    test test{ 1, 1 };
    reconstruct(test);

    REQUIRE(test.x == 0);
    REQUIRE(test.y == 0);
}

TEST_CASE("utility::reconstruct<Args>")
{
    struct test
    {
        test(int x, int y)
            : x(x), y(y) {}

        int x{};
        int y{};
    };

    test test(1, 1);
    reconstruct(test, 0, 0);

    REQUIRE(test.x == 0);
    REQUIRE(test.y == 0);
}

}  // namespace tests_utility
