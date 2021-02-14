TEST_CASE("algorithm::detail::len")
{
    const char t1[]   = "Test";
    const char* t2    = "Test";
    const wchar_t* t3 = L"Test";

    REQUIRE(detail::len(t1) == 4);
    REQUIRE(detail::len(t2) == 4);
    REQUIRE(detail::len(t3) == 4);
}

TEST_CASE("algorithm::trimLeftIf")
{
    std::string t0 = "  -";
    trimLeftIf(t0, isspace);
    REQUIRE(t0 == "-");

    std::string t1 = "xx-";
    trimLeftIf(t1, isalpha);
    REQUIRE(t1 == "-");
}

TEST_CASE("algorithm::trimLeft")
{
    std::string t0 = "  -";
    trimLeft(t0);
    REQUIRE(t0 == "-");
}

TEST_CASE("algorithm::trimLeftCopyIf")
{
    std::string t0 = "  -";
    REQUIRE(trimLeftCopyIf(t0, isspace) == "-");

    std::string t1 = "xx-";
    REQUIRE(trimLeftCopyIf(t1, isalpha) == "-");
}

TEST_CASE("algorithm::trimLeftCopy")
{
    std::string t0 = "  -";
    REQUIRE(trimLeftCopy(t0) == "-");
}

TEST_CASE("algorithm::trimRightIf")
{
    std::string t0 = "-  ";
    trimRightIf(t0, isspace);
    REQUIRE(t0 == "-");

    std::string t1 = "-xx";
    trimRightIf(t1, isalpha);
    REQUIRE(t1 == "-");
}

TEST_CASE("algorithm::trimRight")
{
    std::string t0 = "-  ";
    trimRight(t0);
    REQUIRE(t0 == "-");
}

TEST_CASE("algorithm::trimRightCopyIf")
{
    std::string t0 = "-  ";
    REQUIRE(trimRightCopyIf(t0, isspace) == "-");

    std::string t1 = "-xx";
    REQUIRE(trimRightCopyIf(t1, isalpha) == "-");
}

TEST_CASE("algorithm::trimRightCopy")
{
    std::string t0 = "-  ";
    REQUIRE(trimRightCopy(t0) == "-");
}

TEST_CASE("algorithm::trimIf")
{
    std::string t0 = "  -  ";
    trimIf(t0, isspace);
    REQUIRE(t0 == "-");

    std::string t1 = "xx-xx";
    trimIf(t1, isalpha);
    REQUIRE(t1 == "-");
}

TEST_CASE("algorithm::trim")
{
    std::string t0 = "  -  ";
    trim(t0);
    REQUIRE(t0 == "-");
}

TEST_CASE("algorithm::trimCopyIf")
{
    std::string t0 = "  -  ";
    REQUIRE(trimCopyIf(t0, isspace) == "-");

    std::string t1 = "xx-xx";
    REQUIRE(trimCopyIf(t1, isalpha) == "-");
}

TEST_CASE("algorithm::trimCopy")
{
    std::string t0 = "  -  ";
    REQUIRE(trimCopy(t0) == "-");
}

TEST_CASE("algorithm::toLower")
{
    std::string t0 = "TEst";
    toLower(t0);
    REQUIRE(t0 == "test");
}

TEST_CASE("algorithm::toLowerCopy")
{
    std::string t0 = "TEst";
    REQUIRE(toLowerCopy(t0) == "test");
}

TEST_CASE("algorithm::toUpper")
{
    std::string t0 = "TEst";
    toUpper(t0);
    REQUIRE(t0 == "TEST");
}

TEST_CASE("algorithm::toUpperCopy")
{
    std::string t0 = "TEst";
    REQUIRE(toUpperCopy(t0) == "TEST");
}

TEST_CASE("algorithm::replaceFirst")
{
    std::string t0 = "x|x";
    replaceFirst(t0, "x", "xxx");
    REQUIRE(t0 == "xxx|x");
}

TEST_CASE("algorithm::replaceFirstCopy")
{
    REQUIRE(replaceFirstCopy("x|x"s, "x", "xxx") == "xxx|x");
}

TEST_CASE("algorithm::replaceLast")
{
    std::string t0 = "x|x";
    replaceLast(t0, "x", "xxx");
    REQUIRE(t0 == "x|xxx");
}

TEST_CASE("algorithm::replaceLastCopy")
{
    REQUIRE(replaceLastCopy("x|x"s, "x", "xxx") == "x|xxx");
}

TEST_CASE("algorithm::replaceAll")
{
    std::string t0 = "x|x";
    replaceAll(t0, "x", "xxx");
    REQUIRE(t0 == "xxx|xxx");
}

TEST_CASE("algorithm::replaceAllCopy")
{
    REQUIRE(replaceAllCopy("x|x"s, "x", "xxx") == "xxx|xxx");
}

TEST_CASE("algorithm::split")
{
    const char* del = "|";
    REQUIRE(split(std::string("xxx|xxx"), del) == std::vector<std::string>{ "xxx", "xxx" });

    std::vector<std::string> parts;
    split(std::back_inserter(parts), "xxx|xxx"s, "|");
    REQUIRE(parts[0] == "xxx");
    REQUIRE(parts[1] == "xxx");
}

TEST_CASE("algorithm::join")
{
    REQUIRE(join(std::vector<std::string>{ "xxx", "xxx" }, "|") == "xxx|xxx");
}

TEST_CASE("algorithm::startsWith")
{
    REQUIRE( startsWith(std::string("test"), "test"));
    REQUIRE( startsWith(std::string("test"), std::string("te")));
    REQUIRE( startsWith(std::string("test"), "t"));
    REQUIRE(!startsWith(std::string("test"), "tt"));
    REQUIRE(!startsWith(std::string("test"), "ttest"));
    REQUIRE( startsWith(std::string(""), ""));
    REQUIRE( startsWith(std::string("test"), ""));
}

TEST_CASE("algorithm::endsWith")
{
    REQUIRE( endsWith(std::string("test"), std::string("test")));
    REQUIRE( endsWith(std::string("test"), "st"));
    REQUIRE( endsWith(std::string("test"), "t"));
    REQUIRE(!endsWith(std::string("test"), "tt"));
    REQUIRE(!endsWith(std::string("test"), "testt"));
    REQUIRE( endsWith(std::string(""), ""));
    REQUIRE( endsWith(std::string("test"), ""));
}
