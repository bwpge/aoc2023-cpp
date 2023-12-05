#include "aoc/digit.hpp"
#include "aoc/stringutils.hpp"

#include <array>

namespace aoc {

const auto SPELLED_NUMS = std::to_array<std::string>(
    {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}
);

std::optional<int> decode_digit(std::string s, size_t pos) {
    if (std::isdigit(s[pos])) {
        return s[pos] - '0';
    }

    for (size_t i = 0; i < SPELLED_NUMS.size(); i++) {
        const auto& num = SPELLED_NUMS.at(i);
        if (pos + num.length() > s.length()) {
            continue;
        }

        // get string slice s[pos, pos+num.length()]
        // note: we do actually need string::substr to copy for transform to lowercase.
        // not sure if this is explicitly required, since input has no uppercase letters,
        // but better safe than sorry.
        auto sub = s.substr(pos, num.length());
        to_lower(sub);

        if (num == sub) {
            return static_cast<int>(i);
        }
    }

    return std::optional<int>{};
}

}  // namespace aoc
