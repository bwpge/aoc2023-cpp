#pragma once

#include "aoc/types.hpp"

#include <algorithm>
#include <cctype>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace aoc {

// https://stackoverflow.com/a/67664561
template<typename T>
std::string_view trim_left(const T& s) {
    std::string_view sv{s};
    sv.remove_prefix(std::min(sv.find_first_not_of(" \t\n\r\f\v"), sv.size()));
    return sv;
}

template<typename T>
std::string_view trim_right(const T& s) {
    std::string_view sv{s};
    auto trim_pos = sv.find_last_not_of(" \t\n\r\f\v");
    if (trim_pos == std::string_view::npos) {
        return {};
    }
    sv.remove_suffix(sv.size() - trim_pos - 1);
    return sv;
}

template<typename T>
std::string_view trim(const T& s) {
    auto sv = trim_left(s);
    sv = trim_right(sv);
    return sv;
}

void to_lower(std::string& s);

std::string to_lower_copy(std::string_view sv);

void to_upper(std::string& s);

std::string to_upper_copy(std::string_view sv);

template<StrLike T, Pattern P>
std::string remove(const T& s, const P& pattern) {
    std::string copy{s};
    auto pos = copy.find(pattern);
    while (pos != std::string::npos) {
        copy.erase(pos, 1);
        pos = copy.find(' ');
    }

    return copy;
}

enum class SplitOptions {
    None = 0,
    DiscardEmpty = 1 << 0,
    Trim = 1 << 1,
};

SplitOptions operator|(SplitOptions lhs, SplitOptions rhs);

SplitOptions operator&(SplitOptions lhs, SplitOptions rhs);

std::ostream& operator<<(std::ostream& os, const SplitOptions& opt);

template<Pattern P>
std::optional<std::pair<std::string_view, std::string_view>> split_once(
    std::string_view s,
    P pattern
) {
    if (s.empty()) {
        return {};
    }
    if (details::pattern_len(pattern) == 0) {
        return {{{}, s}};
    }
    auto idx = s.find(pattern);
    if (idx == std::string::npos) {
        return {};
    }

    std::pair<std::string_view, std::string_view> value{
        s.substr(0, idx),
        s.substr(idx + details::pattern_len(pattern)),
    };

    return value;
}

template<Pattern P>
std::vector<std::string_view> split(std::string_view s, P pattern, SplitOptions opts = {}) {
    std::vector<std::string_view> result{};

    std::string_view buf{s};
    decltype(split_once(s, pattern)) splits{};
    while (!buf.empty() && (splits = split_once(buf, pattern)).has_value()) {
        auto [left, right] = splits.value();

        if (static_cast<bool>(opts & SplitOptions::Trim)) {
            left = trim(left);
        }

        bool discard = static_cast<bool>(opts & SplitOptions::DiscardEmpty);
        if (!discard || (discard && !left.empty())) {
            result.push_back(left);
        }

        buf = right;
    }

    if (!result.empty() || !buf.empty()) {
        if (static_cast<bool>(opts & SplitOptions::Trim)) {
            buf = trim(buf);
        }
        if (!buf.empty() || (!static_cast<bool>(opts & SplitOptions::DiscardEmpty))) {
            result.push_back(buf);
        }
    }

    return result;
}

inline bool is_word_char(char c) {
    return static_cast<bool>(std::isalnum(c)) || c == '_';
}

}  // namespace aoc
