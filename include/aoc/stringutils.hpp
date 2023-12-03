#pragma once

#include <algorithm>
#include <cctype>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

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
        return t.length();
    }
}  // namespace details

template<typename T>
concept Pattern = requires(T t) {
    { details::pattern_len(t) } -> std::same_as<details::size_type>;
};

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

template<typename T, typename D>
std::string join(const T& items, D delim) {
    std::ostringstream oss{};
    for (size_t i = 0; i < items.size(); i++) {
        oss << items.at(i);
        if ((i + 1) < items.size()) {
            oss << delim;
        }
    }

    return oss.str();
}

enum class SplitOptions {
    None = 0,
    DiscardEmpty = 1 << 0,
    Trim = 1 << 1,
};

inline SplitOptions operator|(SplitOptions lhs, SplitOptions rhs) {
    using Ty = std::underlying_type_t<SplitOptions>;
    return static_cast<SplitOptions>(static_cast<Ty>(lhs) | static_cast<Ty>(rhs));
}

inline SplitOptions operator&(SplitOptions lhs, SplitOptions rhs) {
    using Ty = std::underlying_type_t<SplitOptions>;
    return static_cast<SplitOptions>(static_cast<Ty>(lhs) & static_cast<Ty>(rhs));
}

inline std::ostream& operator<<(std::ostream& os, const SplitOptions& opt) {
    std::vector<std::string> values{};
    if (opt == SplitOptions::None) {
        return os << "{ None }";
    }

    os << '{';
    if (static_cast<bool>(opt & SplitOptions::DiscardEmpty)) {
        values.emplace_back("DiscardEmpty");
    }
    if (static_cast<bool>(opt & SplitOptions::Trim)) {
        values.emplace_back("Trim");
    }
    auto joined = aoc::join(values, ", ");
    if (joined.empty()) {
        return os << '}';
    }

    return os << ' ' << joined << " }";
}

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

        if (((static_cast<bool>(opts & SplitOptions::DiscardEmpty)) && !left.empty())
            || (!static_cast<bool>(opts & SplitOptions::DiscardEmpty))) {
            result.push_back(left);
        }

        buf = right;
    }

    if (!result.empty()) {
        if (static_cast<bool>(opts & SplitOptions::Trim)) {
            buf = trim(buf);
        }
        if (!buf.empty() || (!static_cast<bool>(opts & SplitOptions::DiscardEmpty))) {
            result.push_back(buf);
        }
    }

    return result;
}

}  // namespace aoc
