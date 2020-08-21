TEST_CASE("locale")
{
    REQUIRE(IsAlnum<char>()('a'));
    REQUIRE(IsAlpha<char>()('a'));
    REQUIRE(IsBlank<char>()(  9));
    REQUIRE(IsCntrl<char>()(  0));
    REQUIRE(IsDigit<char>()('0'));
    REQUIRE(IsGraph<char>()( 33));
    REQUIRE(IsLower<char>()('a'));
    REQUIRE(IsPrint<char>()( 32));
    REQUIRE(IsPunct<char>()( 33));
    REQUIRE(IsSpace<char>()(' '));
    REQUIRE(IsUpper<char>()('A'));
    REQUIRE(IsXDigit<char>()('a'));
    REQUIRE(ToLower<char>()('A') == 'a');
    REQUIRE(ToUpper<char>()('a') == 'A');
}
