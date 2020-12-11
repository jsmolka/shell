TEST_CASE("trimLeftIf")
{
    std::string t0 = "  -";
    trimLeftIf(t0, isspace);
    REQUIRE(t0 == "-");

    std::string t1 = "xx-";
    trimLeftIf(t1, isalpha);
    REQUIRE(t1 == "-");
}

TEST_CASE("trimLeft")
{
    std::string t0 = "  -";
    trimLeft(t0);
    REQUIRE(t0 == "-");
}

TEST_CASE("trimLeftCopyIf")
{
    std::string t0 = "  -";
    REQUIRE(trimLeftCopyIf(t0, isspace) == "-");

    std::string t1 = "xx-";
    REQUIRE(trimLeftCopyIf(t1, isalpha) == "-");
}

TEST_CASE("trimLeftCopy")
{
    std::string t0 = "  -";
    REQUIRE(trimLeftCopy(t0) == "-");
}

TEST_CASE("trimRightIf")
{
    std::string t0 = "-  ";
    trimRightIf(t0, isspace);
    REQUIRE(t0 == "-");

    std::string t1 = "-xx";
    trimRightIf(t1, isalpha);
    REQUIRE(t1 == "-");
}

TEST_CASE("trimRight")
{
    std::string t0 = "-  ";
    trimRight(t0);
    REQUIRE(t0 == "-");
}

TEST_CASE("trimRightCopyIf")
{
    std::string t0 = "-  ";
    REQUIRE(trimRightCopyIf(t0, isspace) == "-");

    std::string t1 = "-xx";
    REQUIRE(trimRightCopyIf(t1, isalpha) == "-");
}

TEST_CASE("trimRightCopy")
{
    std::string t0 = "-  ";
    REQUIRE(trimRightCopy(t0) == "-");
}

TEST_CASE("trimIf")
{
    std::string t0 = "  -  ";
    trimIf(t0, isspace);
    REQUIRE(t0 == "-");

    std::string t1 = "xx-xx";
    trimIf(t1, isalpha);
    REQUIRE(t1 == "-");
}

TEST_CASE("trim")
{
    std::string t0 = "  -  ";
    trim(t0);
    REQUIRE(t0 == "-");
}

TEST_CASE("trimCopyIf")
{
    std::string t0 = "  -  ";
    REQUIRE(trimCopyIf(t0, isspace) == "-");

    std::string t1 = "xx-xx";
    REQUIRE(trimCopyIf(t1, isalpha) == "-");
}

TEST_CASE("trimCopy")
{
    std::string t0 = "  -  ";
    REQUIRE(trimCopy(t0) == "-");
}

TEST_CASE("toLower")
{
    std::string t0 = "TEst";
    toLower(t0);
    REQUIRE(t0 == "test");
}

TEST_CASE("toLowerCopy")
{
    std::string t0 = "TEst";
    REQUIRE(toLowerCopy(t0) == "test");
}

TEST_CASE("toUpper")
{
    std::string t0 = "TEst";
    toUpper(t0);
    REQUIRE(t0 == "TEST");
}

TEST_CASE("toUpperCopy")
{
    std::string t0 = "TEst";
    REQUIRE(toUpperCopy(t0) == "TEST");
}

TEST_CASE("replaceFirst")
{
    std::string t0 = "x|x";
    replaceFirst(t0, "x", "xxx");
    REQUIRE(t0 == "xxx|x");
}

TEST_CASE("replaceFirstCopy")
{
    REQUIRE(replaceFirstCopy("x|x"s, "x", "xxx") == "xxx|x");
}

TEST_CASE("replaceLast")
{
    std::string t0 = "x|x";
    replaceLast(t0, "x", "xxx");
    REQUIRE(t0 == "x|xxx");
}

TEST_CASE("replaceLastCopy")
{
    REQUIRE(replaceLastCopy("x|x"s, "x", "xxx") == "x|xxx");
}

TEST_CASE("replaceAll")
{
    std::string t0 = "x|x";
    replaceAll(t0, "x", "xxx");
    REQUIRE(t0 == "xxx|xxx");
}

TEST_CASE("replaceAllCopy")
{
    REQUIRE(replaceAllCopy("x|x"s, "x", "xxx") == "xxx|xxx");
}

TEST_CASE("split")
{
    REQUIRE(split("xxx|xxx"s, "|") == std::vector<std::string>{ "xxx", "xxx" });

    shell::SmallBuffer<std::string, 10> parts;
    split(std::back_inserter(parts), "xxx|xxx"s, "|");
    REQUIRE(parts[0] == "xxx");
    REQUIRE(parts[1] == "xxx");
}

TEST_CASE("join")
{
    REQUIRE(join(std::vector<std::string>{ "xxx", "xxx" }, "|") == "xxx|xxx");
}
