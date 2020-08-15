TEST_CASE("enumerate")
{
    std::vector<int> x{ 1, 2, 3, 4, 5 };

    for (auto& [index, value] : enumerate(x, 1))
    {
        REQUIRE(index == value);
    }
}
