TEST_CASE("identity")
{
    std::string s = "test";
    REQUIRE(identity(10) == 10);
    REQUIRE(identity(std::forward<std::string>(s)) == s);
}