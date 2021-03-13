enum class Scoped
{
    Flag1 = 1,
    Flag2 = 2,
    Flag4 = 4
};

enum Unscoped
{
    kFlag1 = 1,
    kFlag2 = 2,
    kFlag4 = 4
};

TEST_CASE("operators")
{
    constexpr auto Constexpr = 0 | Scoped::Flag1 | Scoped::Flag2;

    REQUIRE(Constexpr == 3);
    REQUIRE((1 | Scoped::Flag2) == 3);
    REQUIRE((1 | Scoped::Flag4) == 5);
    REQUIRE((0 | Scoped::Flag2 | 1) == 3);
    REQUIRE((0 | Scoped::Flag4 | 1) == 5);
    REQUIRE((kFlag1 | Scoped::Flag2) == 3);
    REQUIRE((kFlag1 | Scoped::Flag4) == 5);
    REQUIRE((0 | Scoped::Flag2 | kFlag1) == 3);
    REQUIRE((0 | Scoped::Flag4 | kFlag1) == 5);
    REQUIRE((0 | Scoped::Flag1 | Scoped::Flag2) == 3);
    REQUIRE((0 | Scoped::Flag1 | Scoped::Flag4) == 5);
    REQUIRE((0 | Scoped::Flag2 | Scoped::Flag1) == 3);
    REQUIRE((0 | Scoped::Flag4 | Scoped::Flag1) == 5);
    REQUIRE((0 | ~Scoped::Flag1) == ~1);
    REQUIRE((3 & Scoped::Flag1) == 1);
    REQUIRE((1 ^ Scoped::Flag1) == 0);
    REQUIRE((Scoped::Flag1 << 1) == Scoped::Flag2);
    REQUIRE(Scoped::Flag1 == kFlag1);
    REQUIRE(Scoped::Flag2 == kFlag2);
    REQUIRE(Scoped::Flag1 == 1);
    REQUIRE(Scoped::Flag2 == 2);

    int x = 0;
    x |=  Scoped::Flag1;
    x |=  Scoped::Flag2;
    x |=  Scoped::Flag4;
    x &= ~Scoped::Flag1;
    x ^=  Scoped::Flag2;
    REQUIRE(x == 4);

    Scoped y = Scoped::Flag1;
    y |= 2;
    y >>= Scoped::Flag1;
    REQUIRE(y == Scoped::Flag1);
}