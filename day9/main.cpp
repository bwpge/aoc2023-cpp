#include "day9.hpp"

#include <filesystem>
#include <numeric>

using day9::Reading;
using Direction = day9::Reading::Direction;

std::vector<Reading> parse_readings(const std::filesystem::path& path) {
    auto input = aoc::open(path);
    std::vector<Reading> result{};

    std::string line{};
    while (std::getline(input, line)) {
        result.emplace_back(Reading::parse(line));
    }

    return result;
}

void part1(const std::vector<Reading>& readings) {
    int64_t sum = std::accumulate(readings.begin(), readings.end(), 0LL, [](auto a, auto b) {
        return a + b.analyze();
    });

    fmt::println("Part 1: {}", sum);
}

void part2(const std::vector<day9::Reading>& readings) {
    int64_t sum = std::accumulate(readings.begin(), readings.end(), 0LL, [](auto a, auto b) {
        return a + b.analyze(Direction::Past);
    });

    fmt::println("Part 2: {}", sum);
}

int main() {
    auto readings = parse_readings("data/day9.txt");
    part1(readings);
    part2(readings);
}
