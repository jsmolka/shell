TEST_CASE("array")
{
    array_t<u32, 2> x1 = { 1, 2 };
    REQUIRE(x1[0] == 1);
    REQUIRE(x1[1] == 2);

    array_t<u32, 2, 4> x2 = { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 } } };
    REQUIRE(x2[0][0] == 1);
    REQUIRE(x2[0][1] == 2);
    REQUIRE(x2[0][2] == 3);
    REQUIRE(x2[0][3] == 4);
    REQUIRE(x2[1][0] == 5);
    REQUIRE(x2[1][1] == 6);
    REQUIRE(x2[1][2] == 7);
    REQUIRE(x2[1][3] == 8);
}
