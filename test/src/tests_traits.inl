TEST_CASE("traits::is_any_of")
{
    REQUIRE( is_any_of_v<u16, u16, u32, u64>);
    REQUIRE( is_any_of_v<u32, u16, u32, u64>);
    REQUIRE( is_any_of_v<u64, u16, u32, u64>);
    REQUIRE(!is_any_of_v<s16, u16, u32, u64>);
    REQUIRE(!is_any_of_v<s32, u16, u32, u64>);
    REQUIRE(!is_any_of_v<s64, u16, u32, u64>);
}

TEST_CASE("traits::is_specialization")
{
    REQUIRE(is_specialization_v<std::vector<u16>, std::vector>);
    REQUIRE(is_specialization_v<std::vector<u32>, std::vector>);
    REQUIRE(is_specialization_v<std::vector<u64>, std::vector>);
}

template<typename T> using has_test1_t = decltype(std::declval<T>().test1());
template<typename T> using has_test2_t = decltype(std::declval<T>().test2());
template<typename T> using has_test3_t = decltype(std::declval<T>().test3(0));
template<typename T> using has_test4_t = decltype(std::declval<T>().test4(""));

TEST_CASE("traits::is_detected")
{
    struct test
    {
        void test1() {}
        void test2(int) {}
        void test3(int) {}
        void test4(int) {}
    };

    REQUIRE( is_detected_v<test, has_test1_t>);
    REQUIRE(!is_detected_v<test, has_test2_t>);
    REQUIRE( is_detected_v<test, has_test3_t>);
    REQUIRE(!is_detected_v<test, has_test4_t>);
}

TEST_CASE("traits::is_iterable")
{
    struct test {};

    REQUIRE( is_iterable_v<std::vector<int>>);
    REQUIRE( is_iterable_v<std::string>);
    REQUIRE(!is_iterable_v<test>);
}