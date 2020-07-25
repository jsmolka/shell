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

template<typename T> using test1_func_t = decltype(std::declval<T>().test1());
template<typename T> using test2_func_t = decltype(std::declval<T>().test2());
template<typename T> using test3_func_t = decltype(std::declval<T>().test3(0));
template<typename T> using test4_func_t = decltype(std::declval<T>().test4(""));

TEST_CASE("traits::detect")
{
    struct Test1
    {
        void test1() {}
        void test2() {}
        void test3() {}
        void test4() {}
    };

    struct Test2
    {
        void test1(int) {}
        void test2(int) {}
        void test3(int) {}
        void test4(const char*) {}
    };

    REQUIRE( detect_v<Test1, test1_func_t>);
    REQUIRE( detect_v<Test1, test2_func_t>);
    REQUIRE(!detect_v<Test1, test3_func_t>);
    REQUIRE(!detect_v<Test1, test4_func_t>);
    REQUIRE(!detect_v<Test2, test1_func_t>);
    REQUIRE(!detect_v<Test2, test2_func_t>);
    REQUIRE( detect_v<Test2, test3_func_t>);
    REQUIRE( detect_v<Test2, test4_func_t>);
}