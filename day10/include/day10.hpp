#pragma once

#include "aoc/aoc.hpp"

namespace day10 {

class Maze {
public:
    using Coord = std::pair<size_t, size_t>;

    enum class Direction {
        North,
        East,
        South,
        West
    };

    static Direction direction(Coord from, Coord to) {
        auto dx = static_cast<int64_t>(to.first) - static_cast<int64_t>(from.first);
        auto dy = static_cast<int64_t>(to.second) - static_cast<int64_t>(from.second);

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

        // store start coordinate
        size_t y{};
        bool found = false;
        for (const auto& row : result._grid) {
            if (!aoc::contains(row, 'S')) {
                ++y;
                continue;
            }

            for (size_t x = 0; x < row.size(); ++x) {
                if (row[x] == 'S') {
                    result._start = {x, y};
                    found = true;
                    break;
                }
            }
            break;
        }

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
    std::vector<Coord> adjacent(Coord pos) {
        std::vector<Coord> result{};

        auto push_coord = [this, &result](const Coord& src, const Coord& dst) {
            if (is_adjacent(src, dst)) {
                result.push_back(dst);
            }
        };

        // north
        if (pos.second > 0) {
            push_coord(pos, {pos.first, pos.second - 1});
        }
        // east
        if (pos.first + 1 <= width()) {
            push_coord(pos, {pos.first + 1, pos.second});
        }
        // south
        if (pos.second + 1 <= height()) {
            push_coord(pos, {pos.first, pos.second + 1});
        }
        // west
        if (pos.first > 0) {
            push_coord(pos, {pos.first - 1, pos.second});
        }

        return result;
    }

    [[nodiscard]]
    char at(Coord pos) const {
        return _grid[pos.second][pos.first];  // use row major
    }

    [[nodiscard]]
    char at(size_t x, size_t y) const {
        return _grid[y][x];  // use row major
    }

    void set_tile(Coord pos, char c) {
        _grid[pos.second][pos.first] = c;
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

private:
    std::vector<std::vector<char>> _grid{};
    Coord _start{};
};

}  // namespace day10
