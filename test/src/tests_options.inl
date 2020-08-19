TEST_CASE("options::bool")
{
    const char* argv[] =
    { 
        "program.exe",
        "-b",
        "-c", "true",
        "true"
    };

    Options options("program", "description");
    options.add({ "--aa", "-a" }, "description", Options::value<bool>()->optional());
    options.add({ "--bb", "-b" }, "description", Options::value<bool>());
    options.add({ "--cc", "-c" }, "description", Options::value<bool>());
    options.add({ "--dd", "-d" }, "description", Options::value<bool>(true));
    options.add({   "ee"       }, "description", Options::value<bool>()->positional());
    
    OptionsResult result = options.parse(sizeof(argv) / sizeof(char*), argv);
    REQUIRE(!result.has("-a"));
    REQUIRE( result.get<bool>("-b"));
    REQUIRE( result.get<bool>("-c"));
    REQUIRE( result.get<bool>("-d"));
    REQUIRE( result.get<bool>("ee"));
}

TEST_CASE("options::int")
{
    const char* argv[] =
    { 
        "program.exe",
        "-b", "1",
        "-c", "2",
        "8"
    };

    Options options("program", "description");
    options.add({ "--aa", "-a" }, "description", Options::value<int>()->optional());
    options.add({ "--bb", "-b" }, "description", Options::value<int>());
    options.add({ "--cc", "-c" }, "description", Options::value<int>());
    options.add({ "--dd", "-d" }, "description", Options::value<int>(4));
    options.add({   "ee"       }, "description", Options::value<int>()->positional());
    
    OptionsResult result = options.parse(sizeof(argv) / sizeof(char*), argv);
    REQUIRE(!result.has("-a"));
    REQUIRE( result.get<int>("-b") == 1);
    REQUIRE( result.get<int>("-c") == 2);
    REQUIRE( result.get<int>("-d") == 4);
    REQUIRE( result.get<int>("ee") == 8);
}

TEST_CASE("options::double")
{
    const char* argv[] =
    { 
        "program.exe",
        "-b", "1.1",
        "-c", "2.1",
        "8.1"
    };

    Options options("program", "description");
    options.add({ "--aa", "-a" }, "description", Options::value<double>()->optional());
    options.add({ "--bb", "-b" }, "description", Options::value<double>());
    options.add({ "--cc", "-c" }, "description", Options::value<double>());
    options.add({ "--dd", "-d" }, "description", Options::value<double>(4.1));
    options.add({   "ee"       }, "description", Options::value<double>()->positional());
    
    OptionsResult result = options.parse(sizeof(argv) / sizeof(char*), argv);
    REQUIRE(!result.has("-a"));
    REQUIRE( result.get<double>("-b") == 1.1);
    REQUIRE( result.get<double>("-c") == 2.1);
    REQUIRE( result.get<double>("-d") == 4.1);
    REQUIRE( result.get<double>("ee") == 8.1);
}

TEST_CASE("options::string")
{
    const char* argv[] =
    { 
        "program.exe",
        "-b", "test1",
        "-c", "test2",
        "test4"
    };

    Options options("program", "description");
    options.add({ "--aa", "-a" }, "description", Options::value<std::string>()->optional());
    options.add({ "--bb", "-b" }, "description", Options::value<std::string>());
    options.add({ "--cc", "-c" }, "description", Options::value<std::string>());
    options.add({ "--dd", "-d" }, "description", Options::value<std::string>("test3"));
    options.add({   "ee"       }, "description", Options::value<std::string>()->positional());
    
    OptionsResult result = options.parse(sizeof(argv) / sizeof(char*), argv);
    REQUIRE(!result.has("-a"));
    REQUIRE( result.get<std::string>("-b") == "test1");
    REQUIRE( result.get<std::string>("-c") == "test2");
    REQUIRE( result.get<std::string>("-d") == "test3");
    REQUIRE( result.get<std::string>("ee") == "test4");
}
