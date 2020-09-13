TEST_CASE("ini::SectionToken")
{
    using Token = ini::detail::SectionToken;
    using Error = ini::ParseError;

    Token token;
    
    token.parse("[test]");
    REQUIRE(token.section == "test");

    token.parse("[test_test]");
    REQUIRE(token.section == "test_test");

    REQUIRE_THROWS_AS(token.parse(""), Error);
    REQUIRE_THROWS_AS(token.parse("["), Error);
    REQUIRE_THROWS_AS(token.parse("]"), Error);
    REQUIRE_THROWS_AS(token.parse("[]"), Error);
    REQUIRE_THROWS_AS(token.parse("[[]"), Error);
    REQUIRE_THROWS_AS(token.parse("[test.]"), Error);
    REQUIRE_THROWS_AS(token.parse("[test-]"), Error);
    REQUIRE_THROWS_AS(token.parse("[test][]"), Error);
    REQUIRE_THROWS_AS(token.parse("[test]test"), Error);
    REQUIRE_THROWS_AS(token.parse("[[test]"), Error);
}

TEST_CASE("ini::CommentToken")
{
    using Token = ini::detail::CommentToken;
    using Error = ini::ParseError;

    Token token;
    
    token.parse("# test");
    REQUIRE(token.token == "#");
    REQUIRE(token.comment == "test");

    token.parse("#test");
    REQUIRE(token.token == "#");
    REQUIRE(token.comment == "test");

    token.parse("#");
    REQUIRE(token.token == "#");
    REQUIRE(token.comment == "");

    token.parse("##");
    REQUIRE(token.token == "#");
    REQUIRE(token.comment == "#");

    token.parse("; test");
    REQUIRE(token.token == ";");
    REQUIRE(token.comment == "test");

    token.parse(";test");
    REQUIRE(token.token == ";");
    REQUIRE(token.comment == "test");

    token.parse(";");
    REQUIRE(token.token == ";");
    REQUIRE(token.comment == "");

    token.parse(";;");
    REQUIRE(token.token == ";");
    REQUIRE(token.comment == ";");

    REQUIRE_THROWS_AS(token.parse(""), Error);
}

TEST_CASE("ini::ValueToken")
{
    using Token = ini::detail::ValueToken;
    using Error = ini::ParseError;

    Token token;
    
    token.parse("test = test");
    REQUIRE(token.key == "test");
    REQUIRE(token.value == "test");

    token.parse("test=test");
    REQUIRE(token.key == "test");
    REQUIRE(token.value == "test");

    token.parse("test_test = test");
    REQUIRE(token.key == "test_test");
    REQUIRE(token.value == "test");

    token.parse("test == test");
    REQUIRE(token.key == "test");
    REQUIRE(token.value == "= test");

    token.parse("test =");
    REQUIRE(token.key == "test");
    REQUIRE(token.value == "");

    REQUIRE_THROWS_AS(token.parse(""), Error);
    REQUIRE_THROWS_AS(token.parse("/=test"), Error);
    REQUIRE_THROWS_AS(token.parse("test.test = test"), Error);
    REQUIRE_THROWS_AS(token.parse("test-test = test"), Error);
    REQUIRE_THROWS_AS(token.parse("test test"), Error);
    REQUIRE_THROWS_AS(token.parse("=test"), Error);
}

TEST_CASE("ini::Ini")
{
    std::vector<std::string> lines = {
        "[test]",
        "# Comment",
        "value1 = 10",
        "value2 = 10.1",
        "value3 = true",
        "value4 = false",
        "value5 = test",
        "value6 = "
    };

    filesystem::write("ini.ini", join(lines, "\n"));

    ini::Ini ini("ini.ini");

    REQUIRE(ini.find<int>("test", "value1") == 10);
    REQUIRE(ini.find<double>("test", "value2") == 10.1);
    REQUIRE(ini.find<bool>("test", "value3") == true);
    REQUIRE(ini.find<bool>("test", "value4") == false);
    REQUIRE(ini.find<std::string>("test", "value5") == "test");
    REQUIRE(ini.find<std::string>("test", "value6") == "");

    REQUIRE_THROWS_AS(ini.find<bool>("test", "value1"), ini::IniError);
    REQUIRE_THROWS_AS(ini.find<int>("test", "value3"), ini::IniError);
}