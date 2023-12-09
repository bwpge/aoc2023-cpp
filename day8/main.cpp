#include "day8.hpp"

#include <filesystem>

using day8::Network;

std::pair<Network, std::string> parse_map(const std::filesystem::path& path) {
    auto input = aoc::open(path);
    std::string directions{};
    std::getline(input, directions);

    Network network{};
    std::string line{};
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        network.add(line);
    }

    return {network, directions};
}

void part1(const std::filesystem::path& path) {
    auto [network, directions] = parse_map(path);
    auto steps = network.steps(directions);

    fmt::println("Part 1: {}", steps);
}

void part2(const std::filesystem::path& path) {
    auto [network, directions] = parse_map(path);
    auto steps = network.steps_parallel(directions);

    fmt::println("Part 2: {}", steps);
}

int main() {
    std::filesystem::path path{"data/day8.txt"};
    part1(path);
    part2(path);
}
