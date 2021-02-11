#define ARGC(argv) (sizeof(argv) / sizeof(char*))

TEST_CASE("options::bool")
{
    char* argv[] =
    { 
        "program.exe",
        "-b",
        "-c",
        "true"
    };

    Options options("program");
    options.add({ "-a", "" }, Options::value<bool>()->optional());
    options.add({ "-b", "" }, Options::value<bool>());
    options.add({ "-c", "" }, Options::value<bool>());
    options.add({ "-d", "" }, Options::value<bool>());
    
    OptionsResult result = options.parse(ARGC(argv), argv);
    REQUIRE(  result.has("-a"));
    REQUIRE( *result.find<bool>("-b"));
    REQUIRE( *result.find<bool>("-c"));
    REQUIRE(!*result.find<bool>("-d"));
}

TEST_CASE("options::int")
{
    char* argv[] =
    { 
        "program.exe",
        "-b", "1",
        "-c", "2",
        "8"
    };

    Options options("program");
    options.add({ "-a", "" }, Options::value<int>()->optional());
    options.add({ "-b", "" }, Options::value<int>());
    options.add({ "-c", "" }, Options::value<int>());
    options.add({ "-d", "" }, Options::value<int>(4));
    options.add({  "e", "" }, Options::value<int>()->positional());
    
    OptionsResult result = options.parse(ARGC(argv), argv);
    REQUIRE(!result.has("-a"));
    REQUIRE(*result.find<int>("-b") == 1);
    REQUIRE(*result.find<int>("-c") == 2);
    REQUIRE(*result.find<int>("-d") == 4);
    REQUIRE(*result.find<int>( "e") == 8);
}

TEST_CASE("options::double")
{
    char* argv[] =
    { 
        "program.exe",
        "-b", "1.1",
        "-c", "2.1",
        "8.1"
    };

    Options options("program");
    options.add({ "-a", "" }, Options::value<double>()->optional());
    options.add({ "-b", "" }, Options::value<double>());
    options.add({ "-c", "" }, Options::value<double>());
    options.add({ "-d", "" }, Options::value<double>(4.1));
    options.add({  "e", "" }, Options::value<double>()->positional());
    
    OptionsResult result = options.parse(ARGC(argv), argv);
    REQUIRE(!result.has("-a"));
    REQUIRE(*result.find<double>("-b") == 1.1);
    REQUIRE(*result.find<double>("-c") == 2.1);
    REQUIRE(*result.find<double>("-d") == 4.1);
    REQUIRE(*result.find<double>( "e") == 8.1);
}

TEST_CASE("options::string")
{
    char* argv[] =
    { 
        "program.exe",
        "-b", "test1",
        "-c", "test2",
        "test4"
    };

    Options options("program");
    options.add({ "-a", "" }, Options::value<std::string>()->optional());
    options.add({ "-b", "" }, Options::value<std::string>());
    options.add({ "-c", "" }, Options::value<std::string>());
    options.add({ "-d", "" }, Options::value<std::string>("test3"));
    options.add({  "e", "" }, Options::value<std::string>()->positional());
    
    OptionsResult result = options.parse(ARGC(argv), argv);
    REQUIRE(!result.has("-a"));
    REQUIRE(*result.find<std::string>("-b") == "test1");
    REQUIRE(*result.find<std::string>("-c") == "test2");
    REQUIRE(*result.find<std::string>("-d") == "test3");
    REQUIRE(*result.find<std::string>( "e") == "test4");
}

TEST_CASE("options::ParseError1")
{
    char* argv1[] = { "program.exe", "-x" };
    char* argv2[] = { "program.exe", "-x", "wrong" };

    Options options1("program");
    Options options2("program");
    Options options3("program");

    options1.add({ "-x", "" }, Options::value<int>());
    options2.add({ "-x", "" }, Options::value<int>());

    CHECK_THROWS_AS(options1.parse(ARGC(argv1), argv1), ParseError);
    //CHECK_THROWS_AS(options2.parse(ARGC(argv2), argv2), ParseError);
}

TEST_CASE("options::ParseError2")
{
    char* argv1[] = { "program.exe" };
    char* argv2[] = { "program.exe" };

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
    options.add({ "--aa,-a", "This is a" }, Options::value<bool>());
    options.add({ "--bb,-b", "This is b" }, Options::value<int>()->optional());
    options.add({ "--cc,-c", "This is c", "data"}, Options::value<double>(1.1));
    options.add({   "dd,d" , "This is d" }, Options::value<std::string>()->positional());
    options.add({   "ee,e" , "This is e" }, Options::value<bool>()->optional());
    options.add({   "ff,f" , "This is f" }, Options::value<std::string>("test")->positional());

    fmt::print(options.help());
}

#undef ARGC
