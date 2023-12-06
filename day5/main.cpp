#include "day5.hpp"

#include <filesystem>
#include <iostream>
#include <queue>

using day5::Almanac;

void part1(const std::filesystem::path& path) {
    auto almanac = Almanac::parse(path, false);

    std::priority_queue<size_t> locs{};
    for (auto seed : almanac.seeds()) {
        auto s = seed.value;
        auto loc = almanac.map_location(s);
        spdlog::trace("seed={}, loc={}, map=[ {} ]", s, loc.back(), fmt::join(loc, " -> "));
        locs.push(loc.back());
    }

    size_t result{};
    while (!locs.empty()) {
        result = locs.top();
        locs.pop();
    }

    std::cout << "Part 1: " << result << '\n';
}

void part2(const std::filesystem::path& path) {
    auto almanac = Almanac::parse(path, true);

    spdlog::info("Searching for lowest location with seed");
    std::optional<size_t> loc{};
    size_t idx{};
    while (!loc.has_value()) {
        loc = almanac.find_seed(idx++);
        if (idx % 1000000 == 0) {
            spdlog::debug("No valid locations, idx={}", idx);
        }
    }

    std::cout << "Part 2: " << idx - 1 << '\n';
}

int main() {
    spdlog::set_level(spdlog::level::trace);
    std::filesystem::path path{"data/day5.txt"};
    part1(path);
    part2(path);
}
