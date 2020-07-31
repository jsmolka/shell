namespace tests_algorithm
{

bool issmall(int x)
{
    return x < 3;
}

TEST_CASE("algorithm::trim_left_if")
{
    std::string t0 = "  -";
    std::string t1 = "ab-";
    std::vector<int> t2 = { 1, 2, 3 };

    trim_left_if(t0, isspace);
    trim_left_if(t1, isalpha);
    trim_left_if(t2, issmall);

    REQUIRE(t0 == "-");
    REQUIRE(t1 == "-");
    REQUIRE(t2 == decltype(t2){ 3 });
}

TEST_CASE("algorithm::trim_left_if_copy")
{
    std::string t0 = "  -";
    std::string t1 = "ab-";
    std::vector<int> t2 = { 1, 2, 3 };

    REQUIRE(trim_left_if_copy(t0, isspace) == "-");
    REQUIRE(trim_left_if_copy(t1, isalpha) == "-");
    REQUIRE(trim_left_if_copy(t2, issmall) == decltype(t2){ 3 });
}

TEST_CASE("algorithm::trim_left")
{
    std::string t0 = "  -";

    trim_left(t0);

    REQUIRE(t0 == "-");
}

TEST_CASE("algorithm::trim_left_copy")
{
    std::string t0 = "  -";

    REQUIRE(trim_left_copy(t0) == "-");
}

TEST_CASE("algorithm::trim_right_if")
{
    std::string t0 = "-  ";
    std::string t1 = "-ba";
    std::vector<int> t2 = { 3, 2, 1 };

    trim_right_if(t0, isspace);
    trim_right_if(t1, isalpha);
    trim_right_if(t2, issmall);

    REQUIRE(t0 == "-");
    REQUIRE(t1 == "-");
    REQUIRE(t2 == decltype(t2){ 3 });
}

TEST_CASE("algorithm::trim_right_if_copy")
{
    std::string t0 = "-  ";
    std::string t1 = "-ba";
    std::vector<int> t2 = { 3, 2, 1 };

    REQUIRE(trim_right_if_copy(t0, isspace) == "-");
    REQUIRE(trim_right_if_copy(t1, isalpha) == "-");
    REQUIRE(trim_right_if_copy(t2, issmall) == decltype(t2){ 3 });
}

TEST_CASE("algorithm::trim_right")
{
    std::string t0 = "-  ";

    trim_right(t0);

    REQUIRE(t0 == "-");
}

TEST_CASE("algorithm::trim_right_copy")
{
    std::string t0 = "-  ";

    REQUIRE(trim_right_copy(t0) == "-");
}

TEST_CASE("algorithm::trim_if")
{
    std::string t0 = "  -  ";
    std::string t1 = "ab-ba";
    std::vector<int> t2 = { 1, 2, 3, 2, 1 };

    trim_if(t0, isspace);
    trim_if(t1, isalpha);
    trim_if(t2, issmall);

    REQUIRE(t0 == "-");
    REQUIRE(t1 == "-");
    REQUIRE(t2 == decltype(t2){ 3 });
}

TEST_CASE("algorithm::trim_if_copy")
{
    std::string t0 = "  -  ";
    std::string t1 = "ab-ba";
    std::vector<int> t2 = { 1, 2, 3, 2, 1 };

    REQUIRE(trim_if_copy(t0, isspace) == "-");
    REQUIRE(trim_if_copy(t1, isalpha) == "-");
    REQUIRE(trim_if_copy(t2, issmall) == decltype(t2){ 3 });
}

TEST_CASE("algorithm::trim")
{
    std::string t0 = "  -  ";

    trim(t0);

    REQUIRE(t0 == "-");
}

TEST_CASE("algorithm::trim_copy")
{
    std::string t0 = "  -  ";

    REQUIRE(trim_copy(t0) == "-");
}

TEST_CASE("algorithm::replace_left")
{
    std::string t0 = "test|test";

    replace_left<std::string>(t0, "test", "");

    REQUIRE(t0 == "|test");
}

TEST_CASE("algorithm::replace_left_copy")
{
    REQUIRE(replace_left_copy<std::string>("test|test", "test", "") == "|test");
}

TEST_CASE("algorithm::replace_right")
{
    std::string t0 = "test|test";

    replace_right<std::string>(t0, "test", "");

    REQUIRE(t0 == "test|");
}

TEST_CASE("algorithm::replace_right_copy")
{
    REQUIRE(replace_right_copy<std::string>("test|test", "test", "") == "test|");
}

TEST_CASE("algorithm::replace")
{
    std::string t0 = "test|test";

    replace<std::string>(t0, "test", "");

    REQUIRE(t0 == "|");
}

TEST_CASE("algorithm::replace_copy")
{
    REQUIRE(replace_copy<std::string>("test|test", "test", "") == "|");
}

TEST_CASE("algorithm::explode")
{
    REQUIRE(explode<std::string>("t.e.s.t", ".") == std::vector<std::string>{ "t", "e", "s", "t" });
    REQUIRE(explode<std::string>("te..st", "..") == std::vector<std::string>{ "te", "st" });
}

TEST_CASE("algorithm::implode")
{
    REQUIRE(implode<std::vector<std::string>, std::string>(std::vector<std::string>{ "t", "e", "s", "t" }, ".") == "t.e.s.t");
    REQUIRE(implode<std::vector<std::string>, std::string>(std::vector<std::string>{ "te", "st" }, "..") == "te..st");
}

}
