TEST_CASE("Singleton")
{
    struct Test : Singleton<Test>
    {
        Test(Token) {};

        int x = 0;
    };

    REQUIRE(&Test::instance() == &Test::instance());

    Test::instance().x = 0;
    REQUIRE(Test::instance().x == 0);

    Test::instance().x = 1;
    REQUIRE(Test::instance().x == 1);
}
