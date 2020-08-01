namespace tests_singleton
{

TEST_CASE("singleton")
{
    struct test : singleton<test>
    {
        test(token) {};

        int x{};
    };

    test::instance().x = 0;
    REQUIRE(test::instance().x == 0);

    test::instance().x = 1;
    REQUIRE(test::instance().x == 1);
}

}  // namespace tests_singleton
