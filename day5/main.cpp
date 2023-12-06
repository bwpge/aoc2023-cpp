#include "day5.hpp"

#include <filesystem>
#include <iostream>
#include <queue>

using day5::Almanac;

size_t min_location(const Almanac& almanac) {
    std::priority_queue<size_t> locs{};
    for (auto seed : almanac.seeds()) {
        auto loc = almanac.map_location(seed);
        spdlog::trace("seed={}, loc={}, map=[ {} ]", seed, loc.back(), fmt::join(loc, " -> "));
        locs.push(loc.back());
    }

    size_t result{};
    while (!locs.empty()) {
        result = locs.top();
        locs.pop();
    }

    return result;
}

void part1(const std::filesystem::path& path) {
    auto almanac = Almanac::parse(path, false);
    auto result = min_location(almanac);
    std::cout << "Part 1: " << result << '\n';
}

void part2(const std::filesystem::path& path) {
    auto almanac = Almanac::parse(path, true);
    auto result = min_location(almanac);
    std::cout << "Part 2: " << result << '\n';
}

int main() {
    spdlog::set_level(spdlog::level::info);
    std::filesystem::path path{"data/day5.txt"};
    part1(path);
    part2(path);
}
