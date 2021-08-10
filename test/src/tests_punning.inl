TEST_CASE("punning::read")
{
    u64 x = 0x0011'AABB'CCDD'EEFF;

    REQUIRE(read<u32>(&x, 0) == 0xCCDD'EEFF);
    REQUIRE(read<u32>(&x, 4) == 0x0011'AABB);
}

TEST_CASE("punning::write")
{
    u64 x = 0;
    write<u32>(&x, 0, 0xCCDD'EEFF);
    write<u32>(&x, 4, 0x0011'AABB);
    REQUIRE(x == 0x0011'AABB'CCDD'EEFF);
}