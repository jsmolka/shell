TEST_CASE("iterator")
{
    std::vector<int> values = { 1, 2, 3 };
    int expected = values.back();

    for (const auto& value : reversed(values))
        REQUIRE(value == expected--);
}
