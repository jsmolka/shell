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

TEST_CASE("parse_primitive")
{
    REQUIRE(*parse_primitive<s32>("            -2147483648"s) ==             -2147483648 );
    REQUIRE(*parse_primitive<s32>("             2147483647"s) ==              2147483647 );
    REQUIRE(*parse_primitive<u32>("             4294967295"s) ==              4294967295 );
    REQUIRE(*parse_primitive<s64>("   -9223372036854775808"s) ==    -9223372036854775808 );
    REQUIRE(*parse_primitive<s64>("    9223372036854775807"s) ==     9223372036854775807 );
    REQUIRE(*parse_primitive<u64>("   18446744073709551615"s) ==    18446744073709551615 );
    REQUIRE(*parse_primitive<f32>("        1.175494351e-38"s) ==         1.175494351e-38f);
    REQUIRE(*parse_primitive<f32>("        3.402823466e+38"s) ==         3.402823466e+38f);
    REQUIRE(*parse_primitive<f64>("2.2250738585072014e-308"s) == 2.2250738585072014e-308 );
    REQUIRE(*parse_primitive<f64>("1.7976931348623158e+308"s) == 1.7976931348623158e+308 );
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
