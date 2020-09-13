TEST_CASE("SectionToken")
{
    using Token = detail::SectionToken;
    using Error = IniParseError;

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

TEST_CASE("CommentToken")
{
    using Token = detail::CommentToken;
    using Error = IniParseError;

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

TEST_CASE("ValueToken")
{
    using Token = detail::ValueToken;
    using Error = IniParseError;

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

//basic_istream& __CLR_OR_THIS_CALL operator>>(short& _Val) { // extract a short
//    ios_base::iostate _Err = ios_base::goodbit;
//    const sentry _Ok(*this);
//
//    if (_Ok) { // state okay, use facet to extract
//        _TRY_IO_BEGIN
//            long _Lval;
//        _STD use_facet<_Nget>(this->getloc()).get(*this, {}, *this, _Err, _Lval);
//        if (_Lval < SHRT_MIN) {
//            _Err |= ios_base::failbit;
//            _Val = SHRT_MIN;
//        } else if (_Lval > SHRT_MAX) {
//            _Err |= ios_base::failbit;
//            _Val = SHRT_MAX;
//        } else {
//            _Val = static_cast<short>(_Lval);
//        }
//        _CATCH_IO_END
//    }
//
//    _Myios::setstate(_Err);
//    return *this;
//}

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