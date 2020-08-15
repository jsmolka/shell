TEST_CASE("reconstruct")
{
    struct Test
    {
        int x = 0;
        int y = 0;
    };

    Test test{1, 1};
    REQUIRE(test.x == 1);
    REQUIRE(test.y == 1);

    reconstruct(test);
    REQUIRE(test.x == 0);
    REQUIRE(test.y == 0);
}

TEST_CASE("reconstruct<Args>")
{
    struct Test
    {
        Test(int x, int y)
            : x(x), y(y) {}

        int x = 0;
        int y = 0;
    };

    Test test(1, 1);
    REQUIRE(test.x == 1);
    REQUIRE(test.y == 1);

    reconstruct(test, 0, 0);
    REQUIRE(test.x == 0);
    REQUIRE(test.y == 0);
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
