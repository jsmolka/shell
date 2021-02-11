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

TEST_CASE("mp::head")
{
    REQUIRE(std::is_same_v<mp::head_t<bool, long, char>, bool>);
}

TEST_CASE("mp::head_element")
{
    REQUIRE(mp::head_element(0, 1, 2) == 0);
}
TEST_CASE("mp::tail")
{
    REQUIRE(std::is_same_v<mp::tail_t<bool, long, char>, char>);
}

TEST_CASE("mp::tail_element")
{
    REQUIRE(mp::tail_element(0, 1, 2) == 2);
}
