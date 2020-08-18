TEST_CASE("bit::bits")
{
    REQUIRE(bit::bits_v< u8> ==  8);
    REQUIRE(bit::bits_v<u16> == 16);
    REQUIRE(bit::bits_v<u32> == 32);
}

TEST_CASE("bit::seq")
{
    REQUIRE(bit::seq< 0, 8>(0x0000'00FF) == 0xFF);
    REQUIRE(bit::seq< 8, 8>(0x0000'FF00) == 0xFF);
    REQUIRE(bit::seq<16, 8>(0x00FF'0000) == 0xFF);
}

TEST_CASE("bit::sign_ex")
{
    REQUIRE(bit::sign_ex< 1>(0x0000'0001) == 0xFFFF'FFFF);
    REQUIRE(bit::sign_ex<16>(0x0000'8000) == 0xFFFF'8000);
    REQUIRE(bit::sign_ex<32>(0x0000'0001) == 0x0000'0001);
}

TEST_CASE("bit::sar")
{
    REQUIRE(bit::sar(0x8000'0000,  3) == 0xF000'0000);
    REQUIRE(bit::sar(0x8000'0000,  7) == 0xFF00'0000);
    REQUIRE(bit::sar(0x8000'0000, 31) == 0xFFFF'FFFF);
}

TEST_CASE("bit::shr")
{
    REQUIRE(bit::shr(0x8000'0000,  3) == 0x1000'0000);
    REQUIRE(bit::shr(0x8000'0000,  7) == 0x0100'0000);
    REQUIRE(bit::shr(0x8000'0000, 31) == 0x0000'0001);
}

TEST_CASE("bit::ror")
{
    REQUIRE(bit::ror(0x0000'00FF,  0) == 0x0000'00FF);
    REQUIRE(bit::ror(0x0000'00FF,  8) == 0xFF00'0000);
    REQUIRE(bit::ror(0x0000'00FF, 32) == 0x0000'00FF);
}

TEST_CASE("bit::rol")
{
    REQUIRE(bit::rol(0x0000'00FF,  0) == 0x0000'00FF);
    REQUIRE(bit::rol(0xFF00'0000,  8) == 0x0000'00FF);
    REQUIRE(bit::rol(0x0000'00FF, 32) == 0x0000'00FF);
}

TEST_CASE("bit::bswap")
{
    REQUIRE(bit::bswap(0x0000'0001) == 0x0100'0000);
    REQUIRE(bit::bswap(0x0000'ABCD) == 0xCDAB'0000);
    REQUIRE(bit::bswap(0xABCD'ABCD) == 0xCDAB'CDAB);
}

TEST_CASE("bit::ctz")
{
    REQUIRE(bit::ctz(0x0000'0001) ==  0);
    REQUIRE(bit::ctz(0x0000'1000) == 12);
    REQUIRE(bit::ctz(0x8000'0000) == 31);
}

TEST_CASE("bit::clz")
{
    REQUIRE(bit::clz(0x8000'0000) ==  0);
    REQUIRE(bit::clz(0x0008'0000) == 12);
    REQUIRE(bit::clz(0x0000'0001) == 31);
}

TEST_CASE("bit::popcnt")
{
    REQUIRE(bit::popcnt(0x0000'0000) ==  0);
    REQUIRE(bit::popcnt(0x0000'0001) ==  1);
    REQUIRE(bit::popcnt(0xFFFF'FFFF) == 32);
}

template<typename T>
bool compare(T value, const std::vector<unsigned>& expected)
{
    auto iterator = bit::iterate(value);

    return std::vector<unsigned>(iterator.begin(), iterator.end()) == expected;
}

TEST_CASE("bit::iterate")
{
    REQUIRE(compare(0x0000'0001, { 0 }));
    REQUIRE(compare(0x0001'0001, { 0, 16 }));
    REQUIRE(compare(0x8001'0001, { 0, 16, 31 }));
}
