TEST_CASE("iterator")
{
    std::vector<int> values = { 1, 2, 3 };
    int expected = values.back();

    for (const auto& value : reversed(values))
        REQUIRE(value == expected--);

    uint x = 0;
    for (auto& v : PointerRange(reinterpret_cast<u8*>(&x), 4))
    {
        v++;
    }
    REQUIRE(x == 0x01010101);
}
