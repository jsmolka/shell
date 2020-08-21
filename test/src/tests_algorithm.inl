bool issmall(int x)
{
    return x < 3;
}

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

    std::string t2;
    trimLeftCopyIf(std::back_inserter(t2), t0, isspace);
    REQUIRE(t2 == "-");

    std::string t3;
    trimLeftCopyIf(std::back_inserter(t3), t1, isalpha);
    REQUIRE(t3 == "-");
}

TEST_CASE("trimLeftCopy")
{
    std::string t0 = "  -";
    REQUIRE(trimLeftCopy(t0) == "-");

    std::string t1;
    trimLeftCopy(std::back_inserter(t1), t0);
    REQUIRE(t1 == "-");
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

    std::string t2;
    trimRightCopyIf(std::back_inserter(t2), t0, isspace);
    REQUIRE(t2 == "-");

    std::string t3;
    trimRightCopyIf(std::back_inserter(t3), t1, isalpha);
    REQUIRE(t3 == "-");
}

TEST_CASE("trimRightCopy")
{
    std::string t0 = "-  ";
    REQUIRE(trimRightCopy(t0) == "-");

    std::string t1;
    trimRightCopy(std::back_inserter(t1), t0);
    REQUIRE(t1 == "-");
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

    std::string t2;
    trimCopyIf(std::back_inserter(t2), t0, isspace);
    REQUIRE(t2 == "-");

    std::string t3;
    trimCopyIf(std::back_inserter(t3), t1, isalpha);
    REQUIRE(t3 == "-");
}

TEST_CASE("trimCopy")
{
    std::string t0 = "  -  ";
    REQUIRE(trimCopy(t0) == "-");

    std::string t1;
    trimCopy(std::back_inserter(t1), t0);
    REQUIRE(t1 == "-");
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

    std::string t1;
    toLowerCopy(std::back_inserter(t1), t0);
    REQUIRE(t1 == "test");
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

    std::string t1;
    toUpperCopy(std::back_inserter(t1), t0);
    REQUIRE(t1 == "TEST");
}

TEST_CASE("replaceFirst")
{
    std::string t0 = "test|test";
    replaceFirst(t0, "test", "");
    REQUIRE(t0 == "|test");
}

TEST_CASE("replaceFirstCopy")
{
    REQUIRE(replaceFirstCopy("test|test"s, "test", "") == "|test");
}

TEST_CASE("replaceLast")
{
    std::string t0 = "test|test";
    replaceLast(t0, "test", "");
    REQUIRE(t0 == "test|");
}

TEST_CASE("replaceLastCopy")
{
    REQUIRE(replaceLastCopy("test|test"s, "test", "") == "test|");
}

TEST_CASE("replaceAll")
{
    std::string t0 = "test|test";
    replaceAll(t0, "test", "");
    REQUIRE(t0 == "|");
}

TEST_CASE("replaceAllCopy")
{
    REQUIRE(replaceAllCopy("test|test"s, "test", "") == "|");
}

TEST_CASE("split")
{
    REQUIRE(split("t.e.s.t"s, ".") == std::vector<std::string>{ "t", "e", "s", "t" });
    REQUIRE(split("te..st"s, "..") == std::vector<std::string>{ "te", "st" });
}

TEST_CASE("join")
{
    REQUIRE(join(std::vector<std::string>{ "t", "e", "s", "t" }, ".") == "t.e.s.t");
    REQUIRE(join(std::vector<std::string>{ "te", "st" }, "..") == "te..st");
}
