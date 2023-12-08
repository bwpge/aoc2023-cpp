#include "day7.hpp"

#include <filesystem>

using day7::Hand;
using day7::JOKERS_WILD;
using day7::RuleSet;

std::vector<Hand> parse_hands(const std::filesystem::path& path, const RuleSet& rules) {
    auto input = aoc::open(path);
    std::vector<Hand> hands{};

    std::string line{};
    while (std::getline(input, line)) {
        auto hand = Hand::parse(line, rules);
        hands.push_back(hand);
    }
    std::ranges::sort(hands);

    return hands;
}

size_t winnings(const std::vector<Hand>& hands) {
    size_t result{};
    for (size_t i = 0; i < hands.size(); ++i) {
        auto rank = i + 1;
        result += hands.at(i).bid() * rank;
    }
    return result;
}

void part1(const std::filesystem::path& path) {
    auto hands = parse_hands(path, {});
    auto result = winnings(hands);

    fmt::println("Part 1: {}", result);
}

void part2(const std::filesystem::path& path) {
    auto hands = parse_hands(path, JOKERS_WILD);
    auto result = winnings(hands);

    fmt::println("Part 2: {}", result);
}

int main() {
    std::filesystem::path path{"data/day7.txt"};
    part1(path);
    part2(path);
}
