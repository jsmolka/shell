TEST_CASE("reconstruct")
{
    struct Test1
    {
        int x = 0;
        int y = 0;
    };

    Test1 test1{1, 1};
    REQUIRE(test1.x == 1);
    REQUIRE(test1.y == 1);

    reconstruct(test1);
    REQUIRE(test1.x == 0);
    REQUIRE(test1.y == 0);

    struct Test2
    {
        Test2(int x, int y) : x(x), y(y) {}

        int x = 0;
        int y = 0;
    };

    Test2 test2(1, 1);
    REQUIRE(test2.x == 1);
    REQUIRE(test2.y == 1);

    reconstruct(test2, 0, 0);
    REQUIRE(test2.x == 0);
    REQUIRE(test2.y == 0);
}
