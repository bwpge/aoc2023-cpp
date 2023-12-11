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

class Sample {
public:
    Sample() = default;

    explicit Sample(bool value) : _top_l{value}, _top_r{value}, _bot_l{value}, _bot_r{value} {}

    static Sample from_pipe(char pipe, bool top_left) {
        Sample s{};
        s._top_l = top_left;

        // we know that crossing over a pipe of the main loop is going to change if the quadrant is
        // filled or not. so as long as we are provided a top left value, we can figure out
        // everything else for this sample based on the pipe character.
        switch (pipe) {
            case '|':
                s._top_r = !top_left;
                s._bot_l = top_left;
                s._bot_r = !top_left;
                break;
            case '-':
                s._top_r = top_left;
                s._bot_l = !top_left;
                s._bot_r = !top_left;
                break;
            case 'L':
                s._top_r = !top_left;
                s._bot_l = top_left;
                s._bot_r = top_left;
                break;
            case 'J':
                s._top_r = !top_left;
                s._bot_l = !top_left;
                s._bot_r = !top_left;
                break;
            case '7':
                s._top_r = top_left;
                s._bot_l = !top_left;
                s._bot_r = top_left;
                break;
            case 'F':
                s._top_r = top_left;
                s._bot_l = top_left;
                s._bot_r = !top_left;
                break;
            case '.':
                s._top_r = top_left;
                s._bot_l = top_left;
                s._bot_r = top_left;
                break;
            default:
                aoc::panic("unknown pipe character");
        }

        return s;
    }

    [[nodiscard]]
    bool is_filled() const {
        return _top_l && _top_r && _bot_l && _bot_r;
    }

    [[nodiscard]]
    bool top_left() const {
        return _top_l;
    }

    [[nodiscard]]
    bool top_right() const {
        return _top_r;
    }

    [[nodiscard]]
    bool bottom_left() const {
        return _bot_l;
    }

    [[nodiscard]]
    bool bottom_right() const {
        return _bot_r;
    }

private:
    bool _top_l{};
    bool _top_r{};
    bool _bot_l{};
    bool _bot_r{};
};

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
        result.deduce_start_tile();
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
    size_t enclosed() const {
        // the method we use here is to "subsample" each tile:
        // - each pipe can split the tile into at most 4 quadrants
        // - the only pipes that can split a tile are the main loop (all others ignored)
        // - the subsample grid will only have enclosed/external values (no pipes)
        // - each sample is filled by previous tile's values (prev top right => next top left)
        // - the top left of each row's starting tile is assumed to be false (since the loop cannot
        //   exit the grid)
        // - the final sum is of samples that are entirely filled (we don't care about splits)

        size_t result{};

        // we don't actually need to store the entire grid, only the previous tile in the row.
        // so we can just iterate rows as scanlines and store the previous tile
        for (size_t y = 0; y < height(); ++y) {
            std::optional<Sample> last{};

            for (size_t x = 0; x < width(); ++x) {
                // the top left of this sample is determined from previous sample's top right
                bool top_left = last.has_value() ? last.value().top_right() : false;
                Coord coord{x, y};

                // determine if the sample gets split by a pipe or not
                if (_loop.contains(coord)) {
                    last = Sample::from_pipe(at(coord), top_left);
                } else {
                    last = Sample{top_left};
                }

                if (last.value().is_filled()) {
                    result += 1;
                }
            }
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

    void deduce_start_tile() {
        auto north = _start.y > 0 ? connects_to({_start.x, _start.y - 1}, _start) : false;
        auto east = _start.x < width() + 1 ? connects_to({_start.x + 1, _start.y}, _start) : false;
        auto south =
            _start.y < height() + 1 ? connects_to({_start.x, _start.y + 1}, _start) : false;
        auto west = _start.x > 0 ? connects_to({_start.x - 1, _start.y}, _start) : false;

        int count = static_cast<int>(north) + static_cast<int>(east) + static_cast<int>(south)
            + static_cast<int>(west);
        AOC_ASSERT(count == 2, "can only deduce tile from 2 connections");

        if (north && south) {
            return set_tile(_start, '|');
        }
        if (east && west) {
            return set_tile(_start, '-');
        }
        if (north && east) {
            return set_tile(_start, 'L');
        }
        if (north && west) {
            return set_tile(_start, 'J');
        }
        if (south && west) {
            return set_tile(_start, '7');
        }
        if (south && east) {
            return set_tile(_start, 'F');
        }

        aoc::panic("could not deduce starting tile");
    }

    void traverse_main_loop() {
        std::queue<Coord> nodes{};
        nodes.push(_start);
        std::unordered_set<Coord> seen{};

        while (!nodes.empty()) {
            auto node = nodes.front();
            nodes.pop();
            seen.insert(node);
            _loop.insert(node);

            for (const auto& coord : adjacent(node)) {
                if (!seen.contains(coord)) {
                    nodes.push(coord);
                }
            }
        }
    }

    std::vector<std::vector<char>> _grid{};
    Coord _start{};
    std::unordered_set<Coord> _loop{};
};

}  // namespace day10
