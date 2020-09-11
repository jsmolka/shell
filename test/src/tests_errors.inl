TEST_CASE("Error")
{
    CHECK_THROWS_WITH(throw Error("test1"), "test1");
    CHECK_THROWS_WITH(throw Error("test2"), "test2");
    CHECK_THROWS_WITH(throw Error("test3"), "test3");
}

TEST_CASE("FormattedError")
{
    CHECK_THROWS_WITH(throw FormattedError("test{}", 1), "test1");
    CHECK_THROWS_WITH(throw FormattedError("test{}", 2), "test2");
    CHECK_THROWS_WITH(throw FormattedError("test{}", 3), "test3");
}

TEST_CASE("throwIf")
{
    CHECK_THROWS_WITH(throwIf<FormattedError>(true, "test{}", 1), "test1");
    CHECK_THROWS_WITH(throwIf<FormattedError>(true, "test{}", 2), "test2");
    CHECK_THROWS_WITH(throwIf<FormattedError>(true, "test{}", 3), "test3");
}
