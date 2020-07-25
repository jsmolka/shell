TEST_CASE("mpl::array")
{
    static constexpr auto squares = mpl::array<uint, 10>([](auto x) {
        return static_cast<uint>(x * x);
    });

    REQUIRE(squares[0] == 0);
    REQUIRE(squares[1] == 1);
    REQUIRE(squares[2] == 4);
    REQUIRE(squares[3] == 9);
}
