
#pragma once

#include <ranges>
#include <utility>
#include <vector>

namespace aoc {

template<
    std::ranges::range ContA,
    std::ranges::range ContB,
    typename A = std::ranges::range_value_t<ContA>,
    typename B = std::ranges::range_value_t<ContB>>
std::vector<std::pair<A, B>> zip_copy(const ContA& a, const ContB& b) {
    auto it_a = std::begin(a);
    auto end_a = std::end(a);
    auto it_b = std::begin(b);
    auto end_b = std::end(b);

    std::vector<std::pair<A, B>> result{};
    while (it_a != end_a && it_b != end_b) {
        result.push_back({*it_a, *it_b});
        ++it_a;
        ++it_b;
    }

    return result;
}

template<std::ranges::range Container, typename T = Container::value_type>
bool contains(const Container& cont, const T& value) {
    return std::ranges::find(cont, value) != std::end(cont);
}

}  // namespace aoc
