TEST_CASE("filesystem::format")
{
    filesystem::path path("test");
    REQUIRE(fmt::format("{}", path) == "test");
}

TEST_CASE("filesystem::absolute")
{
    fmt::print("{}\n", filesystem::absolute("relative/path"));
}

TEST_CASE("filesystem::read/write<vector>")
{
    std::vector<char> src = { 0x00, 0x01, 0x02, 0x03, 0x04 };
    std::vector<char> dst;

    REQUIRE(filesystem::write("out1.bin", src) == filesystem::Status::Ok);
    REQUIRE(filesystem::read ("out1.bin", dst) == filesystem::Status::Ok);
    REQUIRE(filesystem::read ("xyz1.bin", dst) == filesystem::Status::BadFile);
    REQUIRE(src == dst);
}

TEST_CASE("filesystem::read/write<array>")
{
    std::array<char, 5> src = { 0x00, 0x01, 0x02, 0x03, 0x04 };
    std::array<char, 5> dst;
    std::array<char, 6> dst2;

    REQUIRE(filesystem::write("out2.bin", src ) == filesystem::Status::Ok);
    REQUIRE(filesystem::read ("out2.bin", dst ) == filesystem::Status::Ok);
    REQUIRE(filesystem::read ("out2.bin", dst2) == filesystem::Status::BadSize);
    REQUIRE(src == dst);
}

TEST_CASE("filesystem::read/write<string>")
{
    std::string src = "test";
    std::string dst;

    REQUIRE(filesystem::write("sub/out3.bin", src) == filesystem::Status::Ok);
    REQUIRE(filesystem::read ("sub/out3.bin", dst) == filesystem::Status::Ok);
    REQUIRE(src == dst);
}

TEST_CASE("filesystem::isValidPath")
{
    #if SHELL_OS_WINDOWS
    REQUIRE( filesystem::isValidPath("dir\\test.exe"));
    REQUIRE( filesystem::isValidPath("dir\\test name.exe"));
    REQUIRE( filesystem::isValidPath("dir/test.exe"));
    REQUIRE( filesystem::isValidPath("C:/dir/test.exe"));
    REQUIRE(!filesystem::isValidPath("dir/test.exe?"));
    REQUIRE(!filesystem::isValidPath("dir/*test.exe"));
    REQUIRE(!filesystem::isValidPath("C:/dir/tes:t.exe"));
    #endif
}
