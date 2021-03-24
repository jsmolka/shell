TEST_CASE("macros")
{
    for (int i = 0; i < 64; ++i)
    {
        int x = 0;

        switch (i)
        {
        SHELL_CASE32( 0, x = kLabel);
        SHELL_CASE32(32, x = 2 * kLabel);

        default:
            x = -1;
            break;
        }

        if (i < 32)
            REQUIRE(i == x);
        else if (i < 64)
            REQUIRE(2 * i == x);
    }
}
