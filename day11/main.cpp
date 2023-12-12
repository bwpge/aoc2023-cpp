#include "day11.hpp"

#include <filesystem>
#include <set>
#include <unordered_set>

using aoc::Coord;
using day11::GalaxyMap;

namespace std {
template<>
struct hash<std::pair<Coord, Coord>> {
    size_t operator()(const std::pair<Coord, Coord>& pair) const {
        size_t seed{};
        aoc::hash_combine(seed, pair.first, pair.second);

        return seed;
    }
};
}  // namespace std

GalaxyMap parse_map(const std::filesystem::path& path) {
    auto input = aoc::open(path);
    std::vector<std::string> lines{};

    std::string line{};
    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    return day11::make_galaxy_map(lines);
}

void part1(const GalaxyMap& map) {
    std::set<Coord> visited{};
    std::unordered_set<std::pair<Coord, Coord>> pairs{};

    for (const auto& c1 : map.galaxy_coords()) {
        visited.insert(c1);
        for (const auto& c2 : visited) {
            if (c1 == c2) {
                continue;
            }
            pairs.insert({c1, c2});
        }
    }

    size_t sum{};
    for (const auto& [from, to] : pairs) {
        // min distance is always (abs(dx) + abs(dy)) because we can only move in cardinal
        // directions (N/E/S/W) and have no restrictions for which cells can be traversed.
        // so moving diagonally is the same cost as moving up/down then left/right
        sum += map.min_distance(from, to);
    }

    fmt::println("Part 1: {}", sum);
}

void part2() {}

int main() {
    auto map = parse_map("data/day11.txt");
    map.expand();

    part1(map);
    part2();
}
