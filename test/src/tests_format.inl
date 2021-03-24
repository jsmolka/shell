TEST_CASE("format")
{
    REQUIRE(format(10) == "10");
    REQUIRE(format("{}") == "{}");
}
