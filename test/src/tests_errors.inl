TEST_CASE("Error")
{
    CHECK_THROWS_WITH(throw Error("test1"), "test1");
    CHECK_THROWS_WITH(throw Error("test2"), "test2");
    CHECK_THROWS_WITH(throw Error("test3"), "test3");
    CHECK_THROWS_WITH(throw Error("test{}", 1), "test1");
    CHECK_THROWS_WITH(throw Error("test{}", 2), "test2");
    CHECK_THROWS_WITH(throw Error("test{}", 3), "test3");
}
