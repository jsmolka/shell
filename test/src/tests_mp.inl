TEST_CASE("mp::nth")
{
    REQUIRE(std::is_same_v<mp::nth_t<0, bool, long, char>, bool>);
    REQUIRE(std::is_same_v<mp::nth_t<1, bool, long, char>, long>);
    REQUIRE(std::is_same_v<mp::nth_t<2, bool, long, char>, char>);
}

TEST_CASE("mp::nth_element")
{
    REQUIRE(mp::nth_element<0>(0, 1, 2) == 0);
    REQUIRE(mp::nth_element<1>(0, 1, 2) == 1);
    REQUIRE(mp::nth_element<2>(0, 1, 2) == 2);
}
