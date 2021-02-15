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

TEST_CASE("RingBuffer::ctor")
{
    RingBuffer<int, 3> x1;
    x1.write(0);
    x1.write(1);
    RingBuffer<int, 3> x2 = x1;
    RingBuffer<int, 3> x3 = { 0, 1 };

    int v = x1.read();
    REQUIRE(v == x2.read());
    REQUIRE(v == x3.read());
    
    v = x1.read();
    REQUIRE(v == x2.read());
    REQUIRE(v == x3.read());
}

TEST_CASE("RingBuffer::operator")
{
    RingBuffer<int, 3> x = { 4, 1, 2, 3 };

    REQUIRE(x[0] == 1);
    REQUIRE(x[1] == 2);
    REQUIRE(x[2] == 3);
}

TEST_CASE("RingBuffer::ForwardIterator")
{
    RingBuffer<int, 3> x;
    x.write(4);
    x.write(1);
    x.write(2);
    x.write(3);

    int y = 0;
    for (const auto& z : x)
    {
        REQUIRE(++y == z);
    }
    REQUIRE(y == 3);

    RingBuffer<int, 3> x2;
    x2.write(1);
    x2.write(2);
    x2.read();

    y = 1;
    for (const auto& z : x2)
    {
        REQUIRE(++y == z);
    }
    REQUIRE(y == 2);
}
