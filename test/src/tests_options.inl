#define ARGC(argv) (sizeof(argv) / sizeof(const char*))

TEST_CASE("options::bool")
{
    const char* argv[] =
    { 
        "program.exe",
        "-b",
        "-g=true",
        "-c",
        "true",
        "false",
        "-k=false",
        "-j=false"
    };

    Options options("program");
    options.add({ "-a", "" }, Options::value<bool>(false));
    options.add({ "-b", "" }, Options::value<bool>());
    options.add({ "-c", "" }, Options::value<bool>());
    options.add({ "-d", "" }, Options::value<bool>()->optional());
    options.add({  "e", "" }, Options::value<bool>()->positional());
    options.add({  "f", "" }, Options::value<bool>()->positional());
    options.add({ "-g", "" }, Options::value<bool>());
    options.add({ "-k", "" }, Options::value<bool>());
    options.add({ "-i", "" }, Options::value<bool>(true));
    options.add({ "-j", "" }, Options::value<bool>(true));

    OptionsResult result = options.parse(ARGC(argv), argv);
    REQUIRE(!*result.find<bool>("-a"));
    REQUIRE( *result.find<bool>("-b"));
    REQUIRE( *result.find<bool>("-c"));
    REQUIRE( !result.find<bool>("-d"));
    REQUIRE( *result.find<bool>( "e"));
    REQUIRE(!*result.find<bool>( "f"));
    REQUIRE( *result.find<bool>("-g"));
    REQUIRE(!*result.find<bool>("-k"));
    REQUIRE( *result.find<bool>("-i"));
    REQUIRE(!*result.find<bool>("-j"));
}

TEST_CASE("options::int")
{
    const char* argv[] =
    { 
        "program.exe",
        "-b", "1",
        "-f=10",
        "-c", "2",
        "8",
        "-g=0x100"
    };

    Options options("program");
    options.add({ "-a", "" }, Options::value<int>()->optional());
    options.add({ "-b", "" }, Options::value<int>());
    options.add({ "-c", "" }, Options::value<int>());
    options.add({ "-d", "" }, Options::value<int>(4));
    options.add({  "e", "" }, Options::value<int>()->positional());
    options.add({ "-f", "" }, Options::value<int>());
    options.add({ "-g", "" }, Options::value<int>());
    
    OptionsResult result = options.parse(ARGC(argv), argv);
    REQUIRE(!result.has("-a"));
    REQUIRE(*result.find<int>("-b") == 1);
    REQUIRE(*result.find<int>("-c") == 2);
    REQUIRE(*result.find<int>("-d") == 4);
    REQUIRE(*result.find<int>( "e") == 8);
    REQUIRE(*result.find<int>("-f") == 10);
    REQUIRE(*result.find<int>("-g") == 0x100);
}

TEST_CASE("options::double")
{
    const char* argv[] =
    { 
        "program.exe",
        "-b", "1.1",
        "-f=0.1",
        "-c", "2.1",
        "8.1",
        "-g=0.1337"
    };

    Options options("program");
    options.add({ "-a", "" }, Options::value<double>()->optional());
    options.add({ "-b", "" }, Options::value<double>());
    options.add({ "-c", "" }, Options::value<double>());
    options.add({ "-d", "" }, Options::value<double>(4.1));
    options.add({  "e", "" }, Options::value<double>()->positional());
    options.add({ "-f", "" }, Options::value<double>());
    options.add({ "-g", "" }, Options::value<double>());
    
    OptionsResult result = options.parse(ARGC(argv), argv);
    REQUIRE(!result.has("-a"));
    REQUIRE(*result.find<double>("-b") == 1.1);
    REQUIRE(*result.find<double>("-c") == 2.1);
    REQUIRE(*result.find<double>("-d") == 4.1);
    REQUIRE(*result.find<double>( "e") == 8.1);
    REQUIRE(*result.find<double>("-f") == 0.1);
    REQUIRE(*result.find<double>("-g") == 0.1337);
}

TEST_CASE("options::string")
{
    const char* argv[] =
    { 
        "program.exe",
        "-b", "test1",
        "-f=test5",
        "-c", "test2",
        "test4",
        "-g=test6",
    };

    Options options("program");
    options.add({ "-a", "" }, Options::value<std::string>()->optional());
    options.add({ "-b", "" }, Options::value<std::string>());
    options.add({ "-c", "" }, Options::value<std::string>());
    options.add({ "-d", "" }, Options::value<std::string>("test3"));
    options.add({  "e", "" }, Options::value<std::string>()->positional());
    options.add({ "-f", "" }, Options::value<std::string>());
    options.add({ "-g", "" }, Options::value<std::string>());
    
    OptionsResult result = options.parse(ARGC(argv), argv);
    REQUIRE(!result.has("-a"));
    REQUIRE(*result.find<std::string>("-b") == "test1");
    REQUIRE(*result.find<std::string>("-c") == "test2");
    REQUIRE(*result.find<std::string>("-d") == "test3");
    REQUIRE(*result.find<std::string>( "e") == "test4");
    REQUIRE(*result.find<std::string>("-f") == "test5");
    REQUIRE(*result.find<std::string>("-g") == "test6");
}

TEST_CASE("options::ParseError1")
{
    const char* argv1[] = { "program.exe", "-x" };
    const char* argv2[] = { "program.exe", "-x", "wrong" };

    Options options1("program");
    Options options2("program");

    options1.add({ "-x", "" }, Options::value<int>());
    options2.add({ "-x", "" }, Options::value<int>());

    CHECK_THROWS_AS(options1.parse(ARGC(argv1), argv1), ParseError);
    CHECK_THROWS_AS(options2.parse(ARGC(argv2), argv2), ParseError);
}

TEST_CASE("options::ParseError2")
{
    const char* argv1[] = { "program.exe" };
    const char* argv2[] = { "program.exe" };

    Options options1("program");
    Options options2("program");

    options1.add({ "-x", "" }, Options::value<int>());
    options2.add({  "x", "" }, Options::value<int>()->positional());

    CHECK_THROWS_AS(options1.parse(ARGC(argv1), argv1), ParseError);
    CHECK_THROWS_AS(options2.parse(ARGC(argv2), argv2), ParseError);
}

TEST_CASE("options::help")
{
    Options options("program");
    options.add({ "-a,--aa", "this is a" }, Options::value<bool>());
    options.add({ "-b,--bb", "this is b" }, Options::value<int>()->optional());
    options.add({ "-c,--cc", "this is c", "data"}, Options::value<double>(1.1));
    options.add({   "d"    , "this is d" }, Options::value<std::string>()->positional());
    options.add({   "f"    , "this is f" }, Options::value<std::string>("test")->positional());

    fmt::print(options.help());
}

#undef ARGC
