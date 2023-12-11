#include "day10.hpp"

#include <filesystem>
#include <string>
#include <vector>

using day10::Maze;

Maze parse_maze(const std::filesystem::path& path) {
    auto input = aoc::open(path);
    std::vector<std::string> lines{};
    std::string line{};
    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    return Maze::parse(lines);
}

void part1(const Maze& maze) {
    auto result = maze.furthest();
    fmt::println("Part 1: {}", result);
}

void part2(const Maze& maze) {
    auto result = maze.enclosed();
    fmt::println("Part 2: {}", result);
}

int main() {
    auto maze = parse_maze("data/day10.txt");
    part1(maze);
    part2(maze);
}
