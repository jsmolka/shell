namespace
{

TEST_CASE("pythonic::enumerate")
{
    std::vector<int> x{ 1, 2, 3, 4, 5 };

    for (const auto& [index, value] : enumerate(x, 1))
        REQUIRE(index == value);
}

}  // namespace
