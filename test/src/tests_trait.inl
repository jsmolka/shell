TEST_CASE("trait::is_any_of")
{
    REQUIRE( trait::is_any_of_v<char, char, short, int, long>);
    REQUIRE( trait::is_any_of_v<short, char, short, int, long>);
    REQUIRE(!trait::is_any_of_v<float, char, short, int, long>);
    REQUIRE(!trait::is_any_of_v<double, char, short, int, long>);
}
