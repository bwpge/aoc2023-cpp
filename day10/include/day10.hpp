#pragma once

#include "aoc/aoc.hpp"

#include <queue>
#include <unordered_set>

namespace day10 {

struct Coord {
    size_t x{};
    size_t y{};

    auto operator<=>(const Coord& other) const = default;
};
}  // namespace day10

namespace std {
template<>
struct hash<day10::Coord> {
    size_t operator()(const day10::Coord& coord) const {
        return std::hash<size_t>()(coord.x) ^ (std::hash<size_t>()(coord.y) << 1);
    }
};
}  // namespace std

namespace day10 {
class Maze {
public:
    enum class Direction {
        North,
        East,
        South,
        West
    };

    static Direction direction(Coord from, Coord to) {
        auto dx = static_cast<int64_t>(to.x) - static_cast<int64_t>(from.x);
        auto dy = static_cast<int64_t>(to.y) - static_cast<int64_t>(from.y);

        AOC_ASSERT(
            (std::abs(dx) + std::abs(dy)) == 1,
            "coordinates must be adjacent in a cardinal direction"
        );

        if (dy > 0) {
            return Direction::South;
        }
        if (dy < 0) {
            return Direction::North;
        }
        if (dx > 0) {
            return Direction::East;
        }
        if (dx < 0) {
            return Direction::West;
        }

        aoc::unreachable();
    }

    static Maze parse(const std::vector<std::string>& lines) {
        AOC_ASSERT(!lines.empty(), "lines must not be empty");

        Maze result{};
        for (const auto& line : lines) {
            result._grid.emplace_back(line.begin(), line.end());
        }

        // verify dimensions
        size_t width{result._grid.front().size()};
        for (const auto& row : result._grid) {
            AOC_ASSERT(row.size() == width, "all rows must have equal size");
        }

        result.find_start_pos();
        result.traverse_main_loop();

        return result;
    }

    [[nodiscard]]
    bool connects_to(Coord pos, Direction dir) const {
        auto c = at(pos);

        // starting tile has to infer from other connections
        if (c == 'S') {
            return true;
        }

        switch (dir) {
            case Direction::North:
                return c == '|' || c == 'L' || c == 'J';
            case Direction::East:
                return c == '-' || c == 'L' || c == 'F';
            case Direction::South:
                return c == '|' || c == '7' || c == 'F';
            case Direction::West:
                return c == '-' || c == 'J' || c == '7';
            default:
                return false;
        }
    }

    [[nodiscard]]
    bool connects_to(Coord src, Coord dst) const {
        auto dir = Maze::direction(src, dst);
        return connects_to(src, dir);
    }

    [[nodiscard]]
    bool is_adjacent(Coord src, Coord dst) const {
        // NOLINTNEXTLINE(readability-suspicious-call-argument)
        return connects_to(src, dst) && connects_to(dst, src);
    }

    [[nodiscard]]
    std::vector<Coord> adjacent(Coord pos) const {
        std::vector<Coord> result{};

        auto push_coord = [this, &result](const Coord& src, const Coord& dst) {
            if (is_adjacent(src, dst)) {
                result.push_back(dst);
            }
        };

        // north
        if (pos.y > 0) {
            push_coord(pos, {pos.x, pos.y - 1});
        }
        // east
        if (pos.x + 1 <= width()) {
            push_coord(pos, {pos.x + 1, pos.y});
        }
        // south
        if (pos.y + 1 <= height()) {
            push_coord(pos, {pos.x, pos.y + 1});
        }
        // west
        if (pos.x > 0) {
            push_coord(pos, {pos.x - 1, pos.y});
        }

        return result;
    }

    [[nodiscard]]
    size_t furthest() const {
        std::unordered_map<Coord, size_t> map{};
        std::queue<std::pair<Coord, size_t>> nodes{};
        nodes.emplace(_start, 0);

        // use bfs to traverse all nodes, use "depth" to track distance
        while (!nodes.empty()) {
            const auto& [node, depth] = nodes.front();
            nodes.pop();

            // skip this node if we already found a better path (this also implies
            // we have a better path for adjacent), otherwise store the new depth
            if (map.contains(node) && map[node] <= depth) {
                continue;
            }
            map[node] = depth;

            // always add adjacent nodes if we stored current depth --
            // this could possibly update connected nodes
            for (const auto& n : adjacent(node)) {
                nodes.emplace(n, depth + 1);
            }
        }

        size_t result{};
        for (const auto& [_, d] : map) {
            result = std::max(result, d);
        }

        return result;
    }

    [[nodiscard]]
    char at(Coord pos) const {
        return _grid[pos.y][pos.x];  // use row major
    }

    [[nodiscard]]
    char at(size_t x, size_t y) const {
        return _grid[y][x];  // use row major
    }

    void set_tile(Coord pos, char c) {
        _grid[pos.y][pos.x] = c;
    }

    void set_tile(size_t x, size_t y, char c) {
        _grid[y][x] = c;
    }

    [[nodiscard]]
    size_t width() const {
        return _grid.front().size();
    }

    [[nodiscard]]
    size_t height() const {
        return _grid.size();
    }

    [[nodiscard]]
    Coord start() const {
        return _start;
    }

private:
    void find_start_pos() {
        size_t y{};
        bool found = false;
        for (const auto& row : _grid) {
            if (!aoc::contains(row, 'S')) {
                ++y;
                continue;
            }

            for (size_t x = 0; x < row.size(); ++x) {
                if (row[x] == 'S') {
                    _start = {x, y};
                    found = true;
                    break;
                }
            }
            break;
        }

        AOC_ASSERT(found, "maze must have a starting tile");
    }

    void traverse_main_loop() {
        std::queue<Coord> nodes{};
        nodes.push(_start);
        std::unordered_set<Coord> seen{};

        while (!nodes.empty()) {
            auto node = nodes.front();
            nodes.pop();
            seen.insert(node);

            for (const auto& coord : adjacent(node)) {
                if (!seen.contains(coord)) {
                    _loop.push_back(coord);
                    nodes.push(coord);
                }
            }
        }
    }

    std::vector<std::vector<char>> _grid{};
    Coord _start{};
    std::list<Coord> _loop{};
};

}  // namespace day10
