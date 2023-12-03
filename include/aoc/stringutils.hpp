#pragma once

#include <algorithm>
#include <string>
#include <string_view>

namespace aoc {

inline void to_lower(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), [](auto c) {
        return static_cast<char>(std::tolower(c));
    });
}

inline std::string to_lower_copy(std::string_view sv) {
    std::string result{sv};
    to_lower(result);

    return result;
}

inline void to_upper(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), [](auto c) {
        return static_cast<char>(std::toupper(c));
    });
}

inline std::string to_upper_copy(std::string_view sv) {
    std::string result{sv};
    to_upper(result);

    return result;
}

}  // namespace aoc
