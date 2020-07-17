TEST_CASE("traits::is_integer")
{
    REQUIRE( traits::is_integer_v<int>);
    REQUIRE( traits::is_integer_v<long>);
    REQUIRE(!traits::is_integer_v<float>);
    REQUIRE(!traits::is_integer_v<double>);
}

TEST_CASE("traits::is_any_of")
{
    REQUIRE( traits::is_any_of_v<int, char, int, float>);
    REQUIRE( traits::is_any_of_v<char, char, int, float>);
    REQUIRE(!traits::is_any_of_v<double, char, int, float>);
    REQUIRE(!traits::is_any_of_v<short, char, int, float>);
}

TEST_CASE("traits::is_size_least")
{
    REQUIRE( traits::is_size_least_v<base::u32, 4>);
    REQUIRE( traits::is_size_least_v<base::u64, 4>);
    REQUIRE(!traits::is_size_least_v<base::u16, 4>);
    REQUIRE(!traits::is_size_least_v<base::u8 , 4>);
}

TEST_CASE("traits::is_size_most")
{
    REQUIRE( traits::is_size_most_v<base::u8 , 4>);
    REQUIRE( traits::is_size_most_v<base::u16, 4>);
    REQUIRE( traits::is_size_most_v<base::u32, 4>);
    REQUIRE(!traits::is_size_most_v<base::u64, 4>);
}

TEST_CASE("traits::is_size_range")
{
    REQUIRE( traits::is_size_range_v<base::u32, 1, 4>);
    REQUIRE( traits::is_size_range_v<base::u8 , 1, 4>);
    REQUIRE(!traits::is_size_range_v<base::u64, 1, 4>);
    REQUIRE(!traits::is_size_range_v<base::u8 , 2, 4>);
}
