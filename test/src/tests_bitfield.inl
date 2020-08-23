TEST_CASE("BitField")
{
    union Field
    {
        uint data;
        BitField< 0, 4, uint> n1;
        BitField< 4, 4, uint> n2;
        BitField< 8, 4, uint> n3;
        BitField<12, 4, uint> n4;
        BitField<16, 4, uint> n5;
        BitField<20, 4, uint> n6;
        BitField<24, 4, uint> n7;
        BitField<28, 4, uint> n8;
    } field = { 0 };

    REQUIRE(field.data == 0);

    field.n1 = 0x1111'1111;
    field.n2 = 0x2222'2222;
    field.n3 = 0x3333'3333;
    field.n4 = 0x4444'4444;
    field.n5 = 0x5555'5555;
    field.n6 = 0x6666'6666;
    field.n7 = 0x7777'7777;
    field.n8 = 0x8888'8888;
    REQUIRE(field.data == 0x8765'4321);

    field.n1 = 0;
    field.n2 = 1;
    field.n3 = 0;
    field.n4 = 1;
    REQUIRE(!static_cast<bool>(field.n1));
    REQUIRE( static_cast<bool>(field.n2));
    REQUIRE(!static_cast<bool>(field.n3));
    REQUIRE( static_cast<bool>(field.n4));

    field.n1 = 0xF;
    field.n2 = 0x0;
    field.n3 = 0xF;
    field.n1 &= 0x3;
    field.n2 |= 0x3;
    field.n3 ^= 0xC;
    REQUIRE(field.n1 == 0x3);
    REQUIRE(field.n2 == 0x3);
    REQUIRE(field.n3 == 0x3);

    field.n1 = 0xF;
    field.n2 = 0x0;
    field.n1.flip();
    field.n2.flip();
    REQUIRE(field.n1 == 0x0);
    REQUIRE(field.n2 == 0xF);

    field.n1 = 0x0;
    field.n1.setUnchecked(0xF);
    REQUIRE(field.n1 == 0xF);
}
