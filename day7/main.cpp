#include "day7.hpp"

#include <filesystem>

using day7::Hand;

void part1(const std::filesystem::path& path) {
    auto input = aoc::open(path);
    std::vector<Hand> hands{};

    std::string line{};
    while (std::getline(input, line)) {
        auto hand = Hand::parse(line);
        hands.push_back(hand);
    }
    std::ranges::sort(hands);

    size_t winnings{};
    for (size_t i = 0; i < hands.size(); ++i) {
        auto rank = i + 1;
        winnings += hands.at(i).bid() * rank;
    }

    fmt::print("Part 1: {}", winnings);
}

void part2() {}

int main() {
    std::filesystem::path path{"data/day7.txt"};
    part1(path);
    part2();
}
