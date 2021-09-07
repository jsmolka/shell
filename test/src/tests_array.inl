TEST_CASE("array::array")
{
    array<u32, 2> x1 = { 1, 2 };
    REQUIRE(x1[0] == 1);
    REQUIRE(x1[1] == 2);

    array<u32, 2, 4> x2 = { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 } } };
    REQUIRE(x2[0][0] == 1);
    REQUIRE(x2[0][1] == 2);
    REQUIRE(x2[0][2] == 3);
    REQUIRE(x2[0][3] == 4);
    REQUIRE(x2[1][0] == 5);
    REQUIRE(x2[1][1] == 6);
    REQUIRE(x2[1][2] == 7);
    REQUIRE(x2[1][3] == 8);
}

TEST_CASE("array::makeArray")
{
    static constexpr auto kArray = makeArray<int, 10>([](auto x) {
        return 2 * static_cast<int>(x);
    });

    for (int i = 0; i < kArray.size(); ++i)
        REQUIRE(kArray[i] == 2 * i);
}
