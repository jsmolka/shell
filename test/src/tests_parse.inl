TEST_CASE("parse::parse<unsigned int>")
{
    REQUIRE(*parse<unsigned int>("10") == 10);
    REQUIRE(*parse<unsigned int>("+10") == 10);
    REQUIRE(*parse<unsigned int>("4294967295") == 4294967295);
    REQUIRE(!parse<unsigned int>("4294967296"));
    REQUIRE(!parse<unsigned int>("-1"));
    REQUIRE(!parse<unsigned int>("x1"));
    REQUIRE(!parse<unsigned int>("12x"));
    REQUIRE(!parse<unsigned int>("12x_"));
    REQUIRE(!parse<unsigned int>("12A"));
}

TEST_CASE("parse::parse base 2")
{
    REQUIRE(*parse<int>("0b01") == 0b01);
    REQUIRE(*parse<int>("0b10") == 0b10);
    REQUIRE(*parse<int>("+0b10") == 0b10);
    REQUIRE(*parse<int>("-0b10") == -0b10);
    REQUIRE(*parse<int>("-0b11") == -0b11);
    REQUIRE(!parse<int>("0b10b"));
    REQUIRE(!parse<int>("0b10X"));
    REQUIRE(!parse<int>("0b122"));
    REQUIRE(!parse<int>("0b122 x"));
    REQUIRE(!parse<int>("0bx122 x"));
    REQUIRE(!parse<int>("0bx122 x"));
    REQUIRE(!parse<int>("0bxa1w22 x"));
}

TEST_CASE("parse::parse base 16")
{
    REQUIRE(*parse<int>("0xFF") == 0xFF);
    REQUIRE(*parse<int>("0xFE") == 0xFE);
    REQUIRE(*parse<int>("+0xFE") == 0xFE);
    REQUIRE(*parse<int>("-0xFE") == -0xFE);
    REQUIRE(*parse<int>("-0xFF") == -0xFF);
    REQUIRE(!parse<int>("-0xFFx"));
    REQUIRE(!parse<int>("0xFxFx"));
    REQUIRE(!parse<int>("0xFF_"));
    REQUIRE(!parse<int>("0xFF2M"));
    REQUIRE(!parse<int>("0xGFF"));
    REQUIRE(!parse<int>("0xGFF aw"));
}

TEST_CASE("parse::parse<double>")
{
    REQUIRE(*parse<double>("1.00") == 1.00);
    REQUIRE(*parse<double>("1.10") == 1.10);
    REQUIRE(*parse<double>("1.01") == 1.01);
    REQUIRE(*parse<double>("-1.01") == -1.01);
    REQUIRE(*parse<double>("+1.01") == 1.01);
    REQUIRE(*parse<double>("1.01e-1") == 0.101);
    REQUIRE(*parse<double>("1.01e+1") == 10.1);
    REQUIRE(*parse<double>("1.01e1") == 10.1);
    REQUIRE(!parse<double>("v+1.01"));
    REQUIRE(!parse<double>("+1.01v"));
    REQUIRE(!parse<double>("+1,01"));
}
