TEST_CASE("toml::TableToken")
{
    using Token = toml::detail::TableToken;
    using Error = toml::ParseError;

    Token token;
    
    token.parse("[test]");
    REQUIRE(token.value == "test");

    REQUIRE_THROWS_AS(token.parse(""), Error);
    REQUIRE_THROWS_AS(token.parse("["), Error);
    REQUIRE_THROWS_AS(token.parse("]"), Error);
    REQUIRE_THROWS_AS(token.parse("[]"), Error);
    REQUIRE_THROWS_AS(token.parse("[[]"), Error);
    REQUIRE_THROWS_AS(token.parse("[test][]"), Error);
    REQUIRE_THROWS_AS(token.parse("[test]test"), Error);
    REQUIRE_THROWS_AS(token.parse("[[test]"), Error);
}

TEST_CASE("toml::CommentToken")
{
    using Token = toml::detail::CommentToken;
    using Error = toml::ParseError;

    Token token;
    
    token.parse("# test");
    REQUIRE(token.value == "test");

    token.parse("#test");
    REQUIRE(token.value == "test");

    token.parse("#");
    REQUIRE(token.value == "");

    token.parse("##");
    REQUIRE(token.value == "#");

    REQUIRE_THROWS_AS(token.parse(""), Error);
}

TEST_CASE("toml::ValueToken")
{
    using Token = toml::detail::ValueToken;
    using Error = toml::ParseError;

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

    token.parse("test.test = test");
    REQUIRE(token.key == "test.test");
    REQUIRE(token.value == "test");

    REQUIRE_THROWS_AS(token.parse(""), Error);
    REQUIRE_THROWS_AS(token.parse("/=test"), Error);
    REQUIRE_THROWS_AS(token.parse("test test"), Error);
    REQUIRE_THROWS_AS(token.parse("test==test"), Error);
    REQUIRE_THROWS_AS(token.parse("test="), Error);
    REQUIRE_THROWS_AS(token.parse("=test"), Error);
}
