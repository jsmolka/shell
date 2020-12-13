#pragma once

#include <fstream>

#include <shell/fmt.h>
#include <shell/parse.h>
#include <shell/traits.h>

#ifdef __cpp_lib_filesystem
#  include <filesystem>
#  define SHELL_FILESYSTEM_NAMESPACE std::filesystem
#  define SHELL_FILESYSTEM_EXPERIMENTAL 0
#else
#  include <experimental/filesystem>
#  define SHELL_FILESYSTEM_NAMESPACE std::experimental::filesystem
#  define SHELL_FILESYSTEM_EXPERIMENTAL 1
#endif

namespace shell::filesystem
{

using namespace SHELL_FILESYSTEM_NAMESPACE;

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

enum class Status
{
    Ok,
    BadFile,
    BadStream,
    BadSize
};

template<typename Container>
Status read(const path& file, Container& dst)
{
    static_assert(sizeof(typename Container::value_type) == 1);

    std::ifstream stream(file, std::ios::binary);

    if (!stream.is_open())
        return Status::BadFile;

    if (!stream)
        return Status::BadStream;

    std::size_t size = file_size(file);

    if constexpr (detail::is_resizable_v<Container>)
        dst.resize(size);

    if (dst.size() != size)
        return Status::BadSize;

    stream.read(reinterpret_cast<char*>(dst.data()), size);

    return Status::Ok;
}

template<typename Container>
Status write(const path& file, const Container& src)
{
    static_assert(sizeof(typename Container::value_type) == 1);

    std::ofstream stream(file, std::ios::binary);

    if (!stream.is_open())
        return Status::BadFile;

    if (!stream)
        return Status::BadStream;

    stream.write(reinterpret_cast<const char*>(src.data()), src.size());

    return Status::Ok;
}

}  // shell::filesystem

template<>
struct fmt::formatter<shell::filesystem::path>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const shell::filesystem::path& path, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), path.string());
    }
};

template<>
inline std::optional<shell::filesystem::path> shell::parse(const std::string& data)
{
    return shell::filesystem::u8path(data);
}
