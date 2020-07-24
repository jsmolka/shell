TEST_CASE("filesystem::executable")
{
    std::cout << "executable: " << filesystem::executable() << std::endl;
    std::cout << "executable_dir: " << filesystem::executable_dir() << std::endl;
}

TEST_CASE("filesystem::read/write vector")
{
    std::vector<char> src = { 0x00, 0x01, 0x02, 0x03, 0x04 };
    std::vector<char> dst;

    filesystem::write("out1.bin", src);
    filesystem::read ("out1.bin", dst);

    REQUIRE(src == dst);
}

TEST_CASE("filesystem::read/write array")
{
    std::array<char, 5> src = { 0x00, 0x01, 0x02, 0x03, 0x04 };
    std::array<char, 5> dst;

    filesystem::write("out2.bin", src);
    filesystem::read ("out2.bin", dst);

    REQUIRE(src == dst);
}
