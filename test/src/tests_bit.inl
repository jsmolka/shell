TEST_CASE("bit::bits")
{
    REQUIRE(bit::bits<base::u8 >() == 8 );
    REQUIRE(bit::bits<base::u16>() == 16);
    REQUIRE(bit::bits<base::u32>() == 32);
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

TEST_CASE("bit::storage_type")
{
    REQUIRE(std::is_same_v<bit::storage_type_t<1>, base::u8> );
    REQUIRE(std::is_same_v<bit::storage_type_t<2>, base::u16>);
    REQUIRE(std::is_same_v<bit::storage_type_t<4>, base::u32>);
    REQUIRE(std::is_same_v<bit::storage_type_t<8>, base::u64>);
}

template<typename T>
bool compare(T value, const std::vector<base::uint>& expected)
{
    auto iterator = bit::iterate(value);

    return std::vector<base::uint>(iterator.begin(), iterator.end()) == expected;
}

TEST_CASE("bit::iterate")
{
    REQUIRE(compare(0x0000'0001, { 0 }));
    REQUIRE(compare(0x0001'0001, { 0, 16 }));
    REQUIRE(compare(0x8001'0001, { 0, 16, 31 }));
}
