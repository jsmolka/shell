TEST_CASE("filesystem::read/write<vector>")
{
    std::vector<char> src = { 0x00, 0x01, 0x02, 0x03, 0x04 };
    std::vector<char> dst;

    REQUIRE(filesystem::write("out1.bin", src));
    REQUIRE(filesystem::read ("out1.bin", dst));
    REQUIRE(src == dst);
}

TEST_CASE("filesystem::read/write<array>")
{
    std::array<char, 5> src = { 0x00, 0x01, 0x02, 0x03, 0x04 };
    std::array<char, 5> dst;

    REQUIRE(filesystem::write("out2.bin", src));
    REQUIRE(filesystem::read ("out2.bin", dst));
    REQUIRE(src == dst);
}

TEST_CASE("filesystem::read/write<string>")
{
    std::string src = "test";
    std::string dst;

    REQUIRE(filesystem::write("out3.bin", src));
    REQUIRE(filesystem::read ("out3.bin", dst));
    REQUIRE(src == dst);
}
