TEST_CASE("delegate")
{
    struct Test
    {
        void inc1()
        {
            value++;
        }

        int inc2(int v)
        {
            value += v;
            return value;
        }

        int value = 0;
    };

    Test test;
    
    Delegate d1(test, &Test::inc1);
    d1();
    REQUIRE(test.value == 1);

    Delegate d2(test, &Test::inc2);
    REQUIRE(d2(2) == 3);
    REQUIRE(test.value == 3);
}
