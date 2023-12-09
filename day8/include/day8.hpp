#pragma once

#include <numeric>
#include "aoc/aoc.hpp"

namespace day8 {

struct Direction {
    std::string name{};
    std::string left{};
    std::string right{};

    static Direction parse(std::string_view s) {
        AOC_ASSERT(aoc::contains(s, '='), "missing required token");
        AOC_ASSERT(aoc::contains(s, '('), "missing required token");
        AOC_ASSERT(aoc::contains(s, ')'), "missing required token");

        Direction result{};
        auto [name, dirs] = aoc::split_once(s, " = ").value();
        auto [left, right] = aoc::split_once(dirs, ", ").value();

        result.name = name;
        result.left = left.substr(1);
        result.right = right.substr(0, right.length() - 1);

        return result;
    }
};

class Network {
    using Key = std::string;
    using Value = std::pair<std::string, std::string>;

public:
    void add(const Direction& direction) {
        _map[direction.name] = {direction.left, direction.right};
    }

    void add(std::string_view direction) {
        auto dir = Direction::parse(direction);
        _map[dir.name] = {dir.left, dir.right};
    }

    size_t steps(
        std::string_view directions,
        const std::string& from = "AAA",
        const std::string& to = "ZZZ"
    ) {
        AOC_ASSERT(
            _map.contains(from) && _map.contains(to),
            "Map must contain start and end destinations"
        );

        std::string current{from};
        size_t count{};

        size_t idx{};
        while (current != to) {
            count++;
            auto c = directions.at(idx);
            const auto& [left, right] = _map[current];
            current = c == 'L' ? left : right;
            idx = (idx + 1) % directions.length();
        }

        return count;
    }

    size_t steps_parallel(
        std::string_view directions,
        const std::string& from_suffix = "A",
        const std::string& to_suffix = "Z"
    ) {
        AOC_ASSERT(
            !(from_suffix.empty() || to_suffix.empty()),
            "Map must contain start and end suffixes"
        );

        std::vector<std::pair<std::string, size_t>> nodes{};
        for (const auto& [node, _] : _map) {
            if (node.ends_with(from_suffix)) {
                auto cycles = cycle_length(directions, node, to_suffix);
                nodes.emplace_back(node, cycles);
            }
        }

        size_t count = 1;
        for (const auto& [_, cycles] : nodes) {
            count = std::lcm(count, cycles);
        }

        return count;
    }

    size_t cycle_length(
        std::string_view directions,
        const std::string& start,
        const std::string& end_suffix
    ) {
        size_t count{};
        size_t idx{};
        std::string current{start};

        while (!current.ends_with(end_suffix)) {
            count++;
            char c = directions.at(idx);
            const auto& [left, right] = _map[current];
            current = c == 'L' ? left : right;
            idx = (idx + 1) % directions.length();
        }

        return count;
    }

private:
    std::unordered_map<Key, Value> _map{};
};
}  // namespace day8
