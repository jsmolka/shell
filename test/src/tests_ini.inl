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
    REQUIRE_THROWS_AS(token.parse("[test][]"), ParseError);
    REQUIRE_THROWS_AS(token.parse("[test]test"), ParseError);
    REQUIRE_THROWS_AS(token.parse("[[test]"), ParseError);
}

TEST_CASE("CommentToken")
{
    detail::CommentToken token;
    
    token.parse("# test");
    REQUIRE(token.comment == "test");

    token.parse("#test");
    REQUIRE(token.comment == "test");

    token.parse("#");
    REQUIRE(token.comment == "");

    token.parse("##");
    REQUIRE(token.comment == "#");

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

TEST_CASE("Ini::parse")
{
    const char* data = R"(
        [test]
        # Comment
        value1 = 10
        value2 = 10.1
        value3 = true
        value4 = false
        value5 = test
        value6 =
    )";

    Ini ini;
    ini.parse(data);

    REQUIRE(*ini.find<int>("test", "value1") == 10);
    REQUIRE(*ini.find<double>("test", "value2") == 10.1);
    REQUIRE(*ini.find<bool>("test", "value3") == true);
    REQUIRE(*ini.find<bool>("test", "value4") == false);
    REQUIRE(*ini.find<std::string>("test", "value5") == "test");
    REQUIRE(*ini.find<std::string>("test", "value6") == "");
    REQUIRE(!ini.find<bool>("test", "value1").has_value());
    REQUIRE(!ini.find<int>("test", "value3").has_value());
}

TEST_CASE("Ini::set")
{
    Ini ini;
    ini.set("test1", "v1", "1");
    ini.set("test2", "v1", "true");
    ini.set("test2", "v2", "2");
    ini.set("test1", "v1", "0");
    REQUIRE(ini.save("test.ini") == filesystem::Status::Ok);

    Ini ini2;
    REQUIRE(ini2.load("test.ini") == filesystem::Status::Ok);
    REQUIRE(*ini2.find<int>("test1", "v1") == 0);
    REQUIRE(*ini2.find<bool>("test2", "v1"));
    REQUIRE(*ini2.find<int>("test2", "v2") == 2);
}
