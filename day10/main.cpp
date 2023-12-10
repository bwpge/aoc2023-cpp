#include "day10.hpp"

#include <filesystem>
#include <string>
#include <vector>

void part1(const std::filesystem::path& path) {
    auto input = aoc::open(path);
    std::vector<std::string> lines{};
    std::string line{};
    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    auto maze = day10::Maze::parse(lines);
    auto graph = day10::Graph::from_maze(maze);
    auto distances = graph.distances();

    size_t d{};
    for (const auto& [_, v] : distances) {
        d = std::max(d, v);
    }

    fmt::println("Part 1: {}", d);
}

void part2() {}

int main() {
    std::filesystem::path path{"data/day10.txt"};
    part1(path);
    part2();
}
