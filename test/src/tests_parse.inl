TEST_CASE("parse::unsigned int")
{
    REQUIRE(*parse<unsigned int>("4294967295") == 4294967295);
    REQUIRE(!parse<unsigned int>("4294967296"));
    REQUIRE(!parse<unsigned int>("-1"));
}
