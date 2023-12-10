#include "day9.hpp"

#include <filesystem>

std::vector<day9::Reading> parse_readings(const std::filesystem::path& path) {
    auto input = aoc::open(path);
    std::vector<day9::Reading> result{};

    std::string line{};
    while (std::getline(input, line)) {
        result.emplace_back(day9::Reading::parse(line));
    }

    return result;
}

void part1(const std::filesystem::path& path) {
    auto readings = parse_readings(path);

    int64_t sum{};
    for (const auto& r : readings) {
        sum += r.predict();
    }

    fmt::println("Part 1: {}", sum);
}

void part2() {}

int main() {
    std::filesystem::path path{"data/day9.txt"};
    part1(path);
    part2();
}
