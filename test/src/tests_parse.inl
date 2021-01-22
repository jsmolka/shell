TEST_CASE("parse::unsigned int")
{
    REQUIRE(*parse<unsigned int>("4294967295") == 4294967295);
    REQUIRE(!parse<unsigned int>("4294967296"));
    REQUIRE(!parse<unsigned int>("-1"));
}

TEST_CASE("parseInt::base2")
{
    REQUIRE(*parseInt<int>("01", 2) == 0b01);
    REQUIRE(*parseInt<int>("10", 2) == 0b10);
}

TEST_CASE("parseInt::base16")
{
    REQUIRE(*parseInt<int>("FF", 16) == 0xFF);
    REQUIRE(*parseInt<int>("FE", 16) == 0xFE);
}
