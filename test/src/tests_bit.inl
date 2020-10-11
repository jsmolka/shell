TEST_CASE("bit::bits")
{
    REQUIRE(bit::bits_v< u8> ==  8);
    REQUIRE(bit::bits_v<u16> == 16);
    REQUIRE(bit::bits_v<u32> == 32);
}

TEST_CASE("bit::ones")
{
    REQUIRE(bit::ones< 0, int>() == 0x0000'0000);
    REQUIRE(bit::ones< 1, int>() == 0x0000'0001);
    REQUIRE(bit::ones< 2, int>() == 0x0000'0003);
    REQUIRE(bit::ones< 3, int>() == 0x0000'0007);
    REQUIRE(bit::ones< 4, int>() == 0x0000'000F);
    REQUIRE(bit::ones< 8, int>() == 0x0000'00FF);
    REQUIRE(bit::ones<32, int>() == 0xFFFF'FFFF);
}

TEST_CASE("bit::ones2")
{
    REQUIRE(bit::ones<int>( 0) == 0x0000'0000);
    REQUIRE(bit::ones<int>( 1) == 0x0000'0001);
    REQUIRE(bit::ones<int>( 2) == 0x0000'0003);
    REQUIRE(bit::ones<int>( 3) == 0x0000'0007);
    REQUIRE(bit::ones<int>( 4) == 0x0000'000F);
    REQUIRE(bit::ones<int>( 8) == 0x0000'00FF);
    REQUIRE(bit::ones<int>(32) == 0xFFFF'FFFF);
}

TEST_CASE("bit::mask")
{
    REQUIRE(bit::mask< 0,  0, int>() == 0x0000'0000);
    REQUIRE(bit::mask< 0,  1, int>() == 0x0000'0001);
    REQUIRE(bit::mask< 1,  1, int>() == 0x0000'0002);
    REQUIRE(bit::mask<16, 16, int>() == 0xFFFF'0000);
    REQUIRE(bit::mask< 0, 32, int>() == 0xFFFF'FFFF);
}

TEST_CASE("bit::mask2")
{
    REQUIRE(bit::mask<int>( 0,  0) == 0x0000'0000);
    REQUIRE(bit::mask<int>( 0,  1) == 0x0000'0001);
    REQUIRE(bit::mask<int>( 1,  1) == 0x0000'0002);
    REQUIRE(bit::mask<int>(16, 16) == 0xFFFF'0000);
    REQUIRE(bit::mask<int>( 0, 32) == 0xFFFF'FFFF);
}

TEST_CASE("bit::seq")
{
    u16 x = 0xDEAD;
    REQUIRE(bit::seq<0, 3>(x) == ((x >> 0) & 0x7));
    REQUIRE(bit::seq<3, 3>(x) == ((x >> 3) & 0x7));
    REQUIRE(bit::seq<6, 1>(x) == ((x >> 6) & 0x1));
    REQUIRE(bit::seq<7, 1>(x) == ((x >> 7) & 0x1));
    REQUIRE(bit::seq<8, 2>(x) == ((x >> 8) & 0x3));
}

TEST_CASE("bit::seq2")
{
    u16 x = 0xDEAD;
    REQUIRE(bit::seq(x, 0, 3) == ((x >> 0) & 0x7));
    REQUIRE(bit::seq(x, 3, 3) == ((x >> 3) & 0x7));
    REQUIRE(bit::seq(x, 6, 1) == ((x >> 6) & 0x1));
    REQUIRE(bit::seq(x, 7, 1) == ((x >> 7) & 0x1));
    REQUIRE(bit::seq(x, 8, 2) == ((x >> 8) & 0x3));
}

TEST_CASE("bit::byte")
{
    REQUIRE(bit::byte<0>(0xAABBCC) == 0xCC);
    REQUIRE(bit::byte<1>(0xAABBCC) == 0xBB);
    REQUIRE(bit::byte<2>(0xAABBCC) == 0xAA);
}

TEST_CASE("bit::byte2")
{
    REQUIRE(bit::byte(0xAABBCC, 0) == 0xCC);
    REQUIRE(bit::byte(0xAABBCC, 1) == 0xBB);
    REQUIRE(bit::byte(0xAABBCC, 2) == 0xAA);
}

TEST_CASE("bit::nibble")
{
    REQUIRE(bit::nibble<0>(0xABC) == 0xC);
    REQUIRE(bit::nibble<1>(0xABC) == 0xB);
    REQUIRE(bit::nibble<2>(0xABC) == 0xA);
}

TEST_CASE("bit::nibble2")
{
    REQUIRE(bit::nibble(0xABC, 0) == 0xC);
    REQUIRE(bit::nibble(0xABC, 1) == 0xB);
    REQUIRE(bit::nibble(0xABC, 2) == 0xA);
}

TEST_CASE("bit::signEx")
{
    REQUIRE(bit::signEx< 1>(0x0000'0001) == 0xFFFF'FFFF);
    REQUIRE(bit::signEx<16>(0x0000'8000) == 0xFFFF'8000);
    REQUIRE(bit::signEx<32>(0x0000'0001) == 0x0000'0001);
}

TEST_CASE("bit::signEx2")
{
    REQUIRE(bit::signEx(0x0000'0001,  1) == 0xFFFF'FFFF);
    REQUIRE(bit::signEx(0x0000'8000, 16) == 0xFFFF'8000);
    REQUIRE(bit::signEx(0x0000'0001, 32) == 0x0000'0001);
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

TEST_CASE("bit::msb")
{
    REQUIRE(bit::msb(0x8000'0000) == 1);
    REQUIRE(bit::msb(0x4000'0000) == 0);
}

TEST_CASE("bit::ror")
{
    REQUIRE(bit::ror(0x0000'00FF,  0) == 0x0000'00FF);
    REQUIRE(bit::ror(0x0000'00FF,  8) == 0xFF00'0000);
    REQUIRE(bit::ror(0x0000'00FF, 32) == 0x0000'00FF);
    REQUIRE(bit::ror(0x8000'8000, 32) == 0x8000'8000);
}

TEST_CASE("bit::rol")
{
    REQUIRE(bit::rol(0x0000'00FF,  0) == 0x0000'00FF);
    REQUIRE(bit::rol(0xFF00'0000,  8) == 0x0000'00FF);
    REQUIRE(bit::rol(0x0000'00FF, 32) == 0x0000'00FF);
    REQUIRE(bit::rol(0x0000'00FF, 32) == 0x0000'00FF);
    REQUIRE(bit::rol(0x8000'8000, 32) == 0x8000'8000);
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
bool compare(T value, const std::vector<uint>& expected)
{
    auto iterator = bit::iterate(value);

    return std::vector<uint>(iterator.begin(), iterator.end()) == expected;
}

TEST_CASE("bit::iterate")
{
    REQUIRE(compare(0x0000'0001, { 0 }));
    REQUIRE(compare(0x0001'0001, { 0, 16 }));
    REQUIRE(compare(0x8001'0001, { 0, 16, 31 }));
}
