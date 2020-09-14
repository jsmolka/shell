TEST_CASE("SectionToken")
{
    detail::SectionToken token;
    
    token.parse("[test]");
    REQUIRE(token.section == "test");

    token.parse("[test_test]");
    REQUIRE(token.section == "test_test");

    REQUIRE_THROWS_AS(token.parse(""), ParseError);
    REQUIRE_THROWS_AS(token.parse("["), ParseError);
    REQUIRE_THROWS_AS(token.parse("]"), ParseError);
    REQUIRE_THROWS_AS(token.parse("[]"), ParseError);
    REQUIRE_THROWS_AS(token.parse("[[]"), ParseError);
    REQUIRE_THROWS_AS(token.parse("[test.]"), ParseError);
    REQUIRE_THROWS_AS(token.parse("[test-]"), ParseError);
    REQUIRE_THROWS_AS(token.parse("[test][]"), ParseError);
    REQUIRE_THROWS_AS(token.parse("[test]test"), ParseError);
    REQUIRE_THROWS_AS(token.parse("[[test]"), ParseError);
}

TEST_CASE("CommentToken")
{
    detail::CommentToken token;
    
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

    REQUIRE_THROWS_AS(token.parse(""), ParseError);
}

TEST_CASE("ValueToken")
{
    detail::ValueToken token;
    
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

    REQUIRE_THROWS_AS(token.parse(""), ParseError);
    REQUIRE_THROWS_AS(token.parse("/=test"), ParseError);
    REQUIRE_THROWS_AS(token.parse("test.test = test"), ParseError);
    REQUIRE_THROWS_AS(token.parse("test-test = test"), ParseError);
    REQUIRE_THROWS_AS(token.parse("test test"), ParseError);
    REQUIRE_THROWS_AS(token.parse("=test"), ParseError);
}

TEST_CASE("Ini")
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

    Ini ini;
    ini.load("ini.ini");

    REQUIRE(*ini.find<int>("test", "value1") == 10);
    REQUIRE(*ini.find<double>("test", "value2") == 10.1);
    REQUIRE(*ini.find<bool>("test", "value3") == true);
    REQUIRE(*ini.find<bool>("test", "value4") == false);
    REQUIRE(*ini.find<std::string>("test", "value5") == "test");
    REQUIRE(*ini.find<std::string>("test", "value6") == "");
    REQUIRE(!ini.find<bool>("test", "value1").has_value());
    REQUIRE(!ini.find<int>("test", "value3").has_value());
}