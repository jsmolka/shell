namespace tests_argparse
{

TEST_CASE("argparse::test")
{
    const char* argv[] = { 
        "program.exe",
        "-b1",
        "-b2", "true",
        "-b3", "false",
        "-i1", "1",
        "-i2", "2",
        "-i2", "3"
    };

    ArgumentParser parser;
    parser.add({ "-b1" }, "desc", std::optional<bool>(false));
    parser.add({ "-b2" }, "desc", std::optional<bool>(false));
    parser.add({ "-b3" }, "desc", std::optional<bool>(true));
    parser.add({ "-i1" }, "desc", std::optional<int>());
    parser.add({ "-i2" }, "desc", std::optional<int>());

    parser.parse(sizeof(argv) / sizeof(char*), argv);

    REQUIRE(parser.get<bool>("-b1") == true);
    REQUIRE(parser.get<bool>("-b2") == true);
    REQUIRE(parser.get<bool>("-b3") == false);
    REQUIRE(parser.get<int>("-i1") == 1);
    REQUIRE(parser.get<int>("-i2") == 3);
}

}  // namespace tests_argparse
