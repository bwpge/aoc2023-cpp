#include "aoc/stringutils.hpp"

#include <cctype>

namespace aoc {

void to_lower(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), [](auto c) {
        return static_cast<char>(std::tolower(c));
    });
}

std::string to_lower_copy(std::string_view sv) {
    std::string result{sv};
    to_lower(result);

    return result;
}

void to_upper(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), [](auto c) {
        return static_cast<char>(std::toupper(c));
    });
}

std::string to_upper_copy(std::string_view sv) {
    std::string result{sv};
    to_upper(result);

    return result;
}

SplitOptions operator|(SplitOptions lhs, SplitOptions rhs) {
    using Ty = std::underlying_type_t<SplitOptions>;
    return static_cast<SplitOptions>(static_cast<Ty>(lhs) | static_cast<Ty>(rhs));
}

SplitOptions operator&(SplitOptions lhs, SplitOptions rhs) {
    using Ty = std::underlying_type_t<SplitOptions>;
    return static_cast<SplitOptions>(static_cast<Ty>(lhs) & static_cast<Ty>(rhs));
}

std::ostream& operator<<(std::ostream& os, const SplitOptions& opt) {
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

}  // namespace aoc
