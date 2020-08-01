namespace tests_mpl
{

TEST_CASE("mpl::array")
{
    static constexpr auto squares = mpl::array<uint, 10>([](auto x) {
        return static_cast<uint>(x * x);
    });

    for (int x = 0; x < 10; ++x)
        REQUIRE(squares[x] == x * x);
}

}  // namespace tests_mpl
