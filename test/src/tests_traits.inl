TEST_CASE("is_any_of")
{
    REQUIRE( is_any_of_v<u16, u16, u32, u64>);
    REQUIRE( is_any_of_v<u32, u16, u32, u64>);
    REQUIRE( is_any_of_v<u64, u16, u32, u64>);
    REQUIRE(!is_any_of_v<s16, u16, u32, u64>);
    REQUIRE(!is_any_of_v<s32, u16, u32, u64>);
    REQUIRE(!is_any_of_v<s64, u16, u32, u64>);
}

TEST_CASE("is_same")
{
    REQUIRE( is_same_v<u16, u16, u16, u16>);
    REQUIRE( is_same_v<u32, u32, u32, u32>);
    REQUIRE(!is_same_v<u16, u16, u32, u16>);
    REQUIRE(!is_same_v<u32, u32, u32, u16>);
}

template<typename T> using test1_t = decltype(std::declval<T>().test1());
template<typename T> using test2_t = decltype(std::declval<T>().test2());
template<typename T> using test3_t = decltype(std::declval<T>().test3(0));
template<typename T> using test4_t = decltype(std::declval<T>().test4(""));

TEST_CASE("is_detected")
{
    struct Test
    {
        void test1() {}
        void test2(int) {}
        void test3(int) {}
        void test4(int) {}
    };

    REQUIRE( is_detected_v<Test, test1_t>);
    REQUIRE(!is_detected_v<Test, test2_t>);
    REQUIRE( is_detected_v<Test, test3_t>);
    REQUIRE(!is_detected_v<Test, test4_t>);
}

TEST_CASE("unqualified")
{
    REQUIRE(std::is_same_v<unqualified_t<int>, int>);
    REQUIRE(std::is_same_v<unqualified_t<const int>, int>);
    REQUIRE(std::is_same_v<unqualified_t<const int&>, int>);
    REQUIRE(std::is_same_v<unqualified_t<int*>, int>);
    REQUIRE(std::is_same_v<unqualified_t<const int*>, int>);
    REQUIRE(std::is_same_v<unqualified_t<int[2]>, int>);
}

TEST_CASE("dereferenced")
{
    struct DereferencedTest
    {
        int operator*() const
        {
            return 0;
        }
    };

    REQUIRE(std::is_same_v<dereferenced_t<int*>, int&>);
    REQUIRE(std::is_same_v<dereferenced_t<DereferencedTest>, int>);
}

enum UnscopedEnum {};
enum class ScopedEnum {};

TEST_CASE("is_scoped_enum")
{
    REQUIRE( is_scoped_enum_v<ScopedEnum>);
    REQUIRE(!is_scoped_enum_v<UnscopedEnum>);
    REQUIRE(!is_scoped_enum_v<int>);
}
