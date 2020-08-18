bool issmall(int x)
{
    return x < 3;
}

TEST_CASE("trimLeftIf")
{
    std::string t0 = "  -";
    std::string t1 = "ab-";
    std::vector<int> t2 = { 1, 2, 3 };

    trimLeftIf(t0, isspace);
    trimLeftIf(t1, isalpha);
    trimLeftIf(t2, issmall);

    REQUIRE(t0 == "-");
    REQUIRE(t1 == "-");
    REQUIRE(t2 == decltype(t2){ 3 });
}

TEST_CASE("trimLeftIfCopy")
{
    std::string t0 = "  -";
    std::string t1 = "ab-";
    std::vector<int> t2 = { 1, 2, 3 };

    REQUIRE(trimLeftIfCopy(t0, isspace) == "-");
    REQUIRE(trimLeftIfCopy(t1, isalpha) == "-");
    REQUIRE(trimLeftIfCopy(t2, issmall) == decltype(t2){ 3 });
}

TEST_CASE("trimLeft")
{
    std::string t0 = "  -";

    trimLeft(t0);

    REQUIRE(t0 == "-");
}

TEST_CASE("trimLeftCopy")
{
    std::string t0 = "  -";

    REQUIRE(trimLeftCopy(t0) == "-");
}

TEST_CASE("trimRightIf")
{
    std::string t0 = "-  ";
    std::string t1 = "-ba";
    std::vector<int> t2 = { 3, 2, 1 };

    trimRightIf(t0, isspace);
    trimRightIf(t1, isalpha);
    trimRightIf(t2, issmall);

    REQUIRE(t0 == "-");
    REQUIRE(t1 == "-");
    REQUIRE(t2 == decltype(t2){ 3 });
}

TEST_CASE("trimRightIfCopy")
{
    std::string t0 = "-  ";
    std::string t1 = "-ba";
    std::vector<int> t2 = { 3, 2, 1 };

    REQUIRE(trimRightIfCopy(t0, isspace) == "-");
    REQUIRE(trimRightIfCopy(t1, isalpha) == "-");
    REQUIRE(trimRightIfCopy(t2, issmall) == decltype(t2){ 3 });
}

TEST_CASE("trimRight")
{
    std::string t0 = "-  ";

    trimRight(t0);

    REQUIRE(t0 == "-");
}

TEST_CASE("trimRightCopy")
{
    std::string t0 = "-  ";

    REQUIRE(trimRightCopy(t0) == "-");
}

TEST_CASE("trimIf")
{
    std::string t0 = "  -  ";
    std::string t1 = "ab-ba";
    std::vector<int> t2 = { 1, 2, 3, 2, 1 };

    trimIf(t0, isspace);
    trimIf(t1, isalpha);
    trimIf(t2, issmall);

    REQUIRE(t0 == "-");
    REQUIRE(t1 == "-");
    REQUIRE(t2 == decltype(t2){ 3 });
}

TEST_CASE("trimIfCopy")
{
    std::string t0 = "  -  ";
    std::string t1 = "ab-ba";
    std::vector<int> t2 = { 1, 2, 3, 2, 1 };

    REQUIRE(trimIfCopy(t0, isspace) == "-");
    REQUIRE(trimIfCopy(t1, isalpha) == "-");
    REQUIRE(trimIfCopy(t2, issmall) == decltype(t2){ 3 });
}

TEST_CASE("trim")
{
    std::string t0 = "  -  ";

    trim(t0);

    REQUIRE(t0 == "-");
}

TEST_CASE("trimCopy")
{
    std::string t0 = "  -  ";

    REQUIRE(trimCopy(t0) == "-");
}

TEST_CASE("replaceLeft")
{
    std::string t0 = "test|test";

    replaceLeft<std::string>(t0, "test", "");

    REQUIRE(t0 == "|test");
}

TEST_CASE("replaceLeftCopy")
{
    REQUIRE(replaceLeftCopy<std::string>("test|test", "test", "") == "|test");
}

TEST_CASE("replaceRight")
{
    std::string t0 = "test|test";

    replaceRight<std::string>(t0, "test", "");

    REQUIRE(t0 == "test|");
}

TEST_CASE("replaceRightCopy")
{
    REQUIRE(replaceRightCopy<std::string>("test|test", "test", "") == "test|");
}

TEST_CASE("replace")
{
    std::string t0 = "test|test";

    replace<std::string>(t0, "test", "");

    REQUIRE(t0 == "|");
}

TEST_CASE("replaceCopy")
{
    REQUIRE(replaceCopy<std::string>("test|test", "test", "") == "|");
}

TEST_CASE("explode")
{
    REQUIRE(explode<std::string>("t.e.s.t", ".") == std::vector<std::string>{ "t", "e", "s", "t" });
    REQUIRE(explode<std::string>("te..st", "..") == std::vector<std::string>{ "te", "st" });
}

TEST_CASE("implode")
{
    REQUIRE(implode<std::vector<std::string>, std::string>(std::vector<std::string>{ "t", "e", "s", "t" }, ".") == "t.e.s.t");
    REQUIRE(implode<std::vector<std::string>, std::string>(std::vector<std::string>{ "te", "st" }, "..") == "te..st");
}

TEST_CASE("contains")
{
    std::vector<int> v1 = { 1, 2, 3, 4 };

    REQUIRE( contains(v1, 1));
    REQUIRE( contains(v1, 2));
    REQUIRE( contains(v1, 3));
    REQUIRE( contains(v1, 4));
    REQUIRE(!contains(v1, 5));
    REQUIRE(!contains(v1, 6));
}
