#pragma once

#include <fstream>

#ifdef __cpp_lib_filesystem
#  include <filesystem>
#else
#  include <experimental/filesystem>
#endif

#include <eggcpt/fmt.h>
#include <eggcpt/parse.h>
#include <eggcpt/traits.h>

namespace eggcpt::filesystem
{

#ifdef __cpp_lib_filesystem
using namespace std::filesystem;
#else
using namespace std::experimental::filesystem;
#endif

namespace detail
{

template<typename T>
using resize_t = decltype(std::declval<T>().resize(1));

template<typename T>
inline constexpr bool is_resizable_v = is_detected_v<T, resize_t>;

inline path base_path = current_path();

}  // namespace detail

inline void setBasePath(const path& path)
{
    detail::base_path = path;
}

inline void setBasePath(int argc, char* argv[])
{
    if (argc > 0) setBasePath(u8path(argv[0]).parent_path());
}

inline path makeAbsolute(const path& path)
{
    return path.is_relative()
        ? detail::base_path / path
        : path;
}

template<typename Container>
bool read(const path& file, Container& dst)
{
    static_assert(sizeof(typename Container::value_type) == 1);

    auto stream = std::ifstream(file, std::ios::binary);
    if (!stream.is_open())
        return false;

    stream.seekg(0, std::ios::end);
    std::size_t size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    if constexpr (detail::is_resizable_v<Container>)
        dst.resize(size);

    if (dst.size() != size)
        return false;

    stream.read(reinterpret_cast<char*>(dst.data()), size);

    return true;
}

template<typename Container>
bool write(const path& file, const Container& src)
{
    static_assert(sizeof(typename Container::value_type) == 1);

    auto stream = std::ofstream(file, std::ios::binary);
    if (!stream.is_open())
        return false;

    stream.write(reinterpret_cast<const char*>(src.data()), src.size());

    return true;
}

}  // namespace eggcpt::filesystem

template<>
struct fmt::formatter<eggcpt::filesystem::path>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const eggcpt::filesystem::path& path, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "{}", path.string());
    }
};

template<>
inline std::optional<eggcpt::filesystem::path> eggcpt::parse(const std::string& data)
{
    return eggcpt::filesystem::u8path(data);
}
