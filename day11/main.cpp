#include "day11.hpp"

#include <filesystem>

using day11::GalaxyMap;

GalaxyMap parse_map(const std::filesystem::path& path) {
    auto input = aoc::open(path);
    std::vector<std::string> lines{};

    std::string line{};
    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    return day11::make_galaxy_map(lines);
}

void part1(GalaxyMap& map) {
    map.set_expansion_factor(1);
    size_t sum{};

    for (const auto& [from, to] : map.galaxy_pairs()) {
        sum += map.min_distance(from, to);
    }

    fmt::println("Part 1: {}", sum);
}

void part2(GalaxyMap& map) {
    map.set_expansion_factor(1'000'000);
    size_t sum{};

    for (const auto& [from, to] : map.galaxy_pairs()) {
        sum += map.min_distance(from, to);
    }

    fmt::println("Part 2: {}", sum);
}

int main() {
    auto map = parse_map("data/day11.txt");
    part1(map);
    part2(map);
}
