#pragma once

#include <string_view>

namespace aoc {

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
