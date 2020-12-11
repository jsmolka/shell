TEST_CASE("iterator")
{
    std::vector<int> values = { 1, 2, 3 };
    int expected = values.back();

    for (const auto& value : reversed(values))
        REQUIRE(value == expected--);

    uint x = 0;
    for (auto& [i, v] : enumerate(makePointerRange(reinterpret_cast<u8*>(&x), 4)))
    {
        v = i;
    }
    REQUIRE(x == 0x0302'0100);
}
