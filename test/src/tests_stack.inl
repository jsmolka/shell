TEST_CASE("stack::Stack")
{
    Stack<int> x = { 1, 2, 3 };
    REQUIRE(x.size() == 3);
    REQUIRE(x.top() == 3);
    REQUIRE(x.popValue() == 3);
    REQUIRE(x.popValue() == 2);
    REQUIRE(x.popValue() == 1);
}
