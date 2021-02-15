TEST_CASE("buffer::FixedBuffer")
{
    FixedBuffer<int, 3> buffer;
    buffer.push_back(0);
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer.capacity() == 3);
    buffer.push_back(1);
    REQUIRE(buffer.size() == 2);
    REQUIRE(buffer.capacity() == 3);
    buffer.push_back(2);
    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer.capacity() == 3);

    FixedBuffer<int, 3> a = { 1, 2 };
    FixedBuffer<int, 3> b(a);
    FixedBuffer<int, 3> c = a;

    REQUIRE(a.size() == 2);
    REQUIRE(b.size() == 2);
    REQUIRE(c.size() == 2);
    REQUIRE(a[0] == 1);
    REQUIRE(b[0] == 1);
    REQUIRE(c[0] == 1);
    REQUIRE(a[1] == 2);
    REQUIRE(b[1] == 2);
    REQUIRE(c[1] == 2);
    REQUIRE(a.front() == 1);
    REQUIRE(a.back() == 2);
}

TEST_CASE("buffer::SmallBuffer")
{
    SmallBuffer<int, 3> buffer;
    int* data = buffer.data();
    buffer.push_back(0);
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer.capacity() == 3);
    REQUIRE(buffer.data() == data);
    buffer.push_back(1);
    REQUIRE(buffer.size() == 2);
    REQUIRE(buffer.capacity() == 3);
    REQUIRE(buffer.data() == data);
    buffer.push_back(2);
    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer.capacity() == 3);
    REQUIRE(buffer.data() == data);
    buffer.push_back(4);
    REQUIRE(buffer.size() == 4);
    REQUIRE(buffer.capacity() == 6);
    REQUIRE(buffer.data() != data);

    SmallBuffer<int, 3> a = { 1, 2 };
    SmallBuffer<int, 3> b(a);
    SmallBuffer<int, 3> c = a;

    REQUIRE(a.size() == 2);
    REQUIRE(b.size() == 2);
    REQUIRE(c.size() == 2);
    REQUIRE(a[0] == 1);
    REQUIRE(b[0] == 1);
    REQUIRE(c[0] == 1);
    REQUIRE(a[1] == 2);
    REQUIRE(b[1] == 2);
    REQUIRE(c[1] == 2);
    REQUIRE(a.front() == 1);
    REQUIRE(a.back() == 2);

    SmallBuffer<int, 3> d(std::move(a));
    SmallBuffer<int, 3> e = std::move(b);

    REQUIRE(d.size() == 2);
    REQUIRE(e.size() == 2);
    REQUIRE(d[0] == 1);
    REQUIRE(e[0] == 1);
    REQUIRE(d[1] == 2);
    REQUIRE(e[1] == 2);

    SmallBuffer<int, 3> f = { 0, 1, 2, 4 };
    auto fp = f.data();
    SmallBuffer<int, 3> g = std::move(f);
    REQUIRE(g.data() == fp);

    int hc = 10;
    SmallBuffer<int, 10> h = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    for (const auto& v : reversed(h))
    {
        REQUIRE(v == --hc);
    }
    REQUIRE(hc == 0);
}
