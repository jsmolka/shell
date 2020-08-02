namespace tests_utility
{

using namespace std::string_literals;

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

TEST_CASE("utility::string_to")
{
    REQUIRE(*string_to<int>("1"s) == 1);
    REQUIRE(*string_to<long>("1"s) == 1l);
    REQUIRE(*string_to<long long>("1"s) == 1ll);
    REQUIRE(*string_to<unsigned long>("1"s) == 1ul);
    REQUIRE(*string_to<unsigned long long>("1"s) == 1ull);
    REQUIRE(*string_to<float>("1.1"s) == 1.1f);
    REQUIRE(*string_to<double>("1.1"s) == 1.1);
    REQUIRE(*string_to<long double>("1.1"s) == 1.1l);
}

}  // namespace tests_utility
