TEST_CASE("argparse::parse")
{
    const char* argv[] =
    { 
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

enum class Value
{
    Invalid,
    Value1,
    Value2
};

template<>
std::optional<Value> argparse::convert(const std::string& data)
{
    if (data == "one") return Value::Value1;
    if (data == "two") return Value::Value2;

    return Value::Invalid;
}

TEST_CASE("argparse::custom")
{
    const char* argv[] =
    {
        "program.exe",
        "-v1", "one",
        "-v2", "two",
        "-v3", "xxx"
    };

    BasicArgumentParser<Value, bool> parser;
    parser.add({ "-v1" }, "desc", std::optional<Value>());
    parser.add({ "-v2" }, "desc", std::optional<Value>());
    parser.add({ "-v3" }, "desc", std::optional<Value>());

    parser.parse(sizeof(argv) / sizeof(char*), argv);

    REQUIRE(parser.get<Value>("-v1") == Value::Value1);
    REQUIRE(parser.get<Value>("-v2") == Value::Value2);
    REQUIRE(parser.get<Value>("-v3") == Value::Invalid);
}
