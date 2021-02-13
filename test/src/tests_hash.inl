TEST_CASE("hash::hash")
{
    u64 seed = hash(0x1234'5678);
    REQUIRE(seed == 0x6A29'5429'B2D6'B891);

    int data[] = {
        0x1234'5678,
        0x1234'5678,
        0x1234'5678,
        0x1234'5678
    };
    seed = hashRange(data);
    REQUIRE(seed == 0xAFFC'E2EE'423B'D28D);
}
