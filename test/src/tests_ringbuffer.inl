TEST_CASE("RingBuffer::read/write")
{
    RingBuffer<int, 3> x;
    x.write(1);
    x.write(2);
    x.write(3);
    x.write(4);

    REQUIRE(x.size() == 3);
    REQUIRE(x.read() == 2);
    REQUIRE(x.read() == 3);
    REQUIRE(x.read() == 4);
    REQUIRE(x.size() == 0);

    x.write(1);
    x.write(2);
    x.write(3);

    REQUIRE(x.front() == 1);
    REQUIRE(x.back() == 3);
}

TEST_CASE("RingBuffer::ForwardIterator")
{
    RingBuffer<int, 3> x;
    x.write(4);
    x.write(1);
    x.write(2);
    x.write(3);

    int y = 1;
    for (const auto& z : x)
    {
        REQUIRE(y++ == z);
    }
}

TEST_CASE("RingBuffer::ReverseIterator")
{
    RingBuffer<int, 3> x;
    x.write(4);
    x.write(1);
    x.write(2);
    x.write(3);

    int y = 3;
    for (const auto& z : reversed(x))
    {
        REQUIRE(y-- == z);
    }
}
