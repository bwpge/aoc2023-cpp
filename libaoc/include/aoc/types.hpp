#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace aoc {

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using usize = std::size_t;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;

template<typename T>
concept CharLike = std::convertible_to<T, char>;

template<typename T>
concept StrLike = std::convertible_to<T, std::string_view>;

namespace details {
    using size_type = std::string_view::size_type;

    template<CharLike T>
    size_type pattern_len(T) {
        return 1;
    }

    template<StrLike T>
    size_type pattern_len(const T& t) {
        return std::string_view(t).length();
    }
}  // namespace details

template<typename T>
concept Pattern = requires(T t) {
    { details::pattern_len(t) } -> std::same_as<details::size_type>;
};

}  // namespace aoc
