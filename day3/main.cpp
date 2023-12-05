#include "aoc/aoc.hpp"

#include <cctype>
#include <filesystem>
#include <iostream>
#include <optional>
#include <ranges>
#include <unordered_map>
#include <vector>

struct Coord {
    size_t col{};
    size_t ln{};

    bool operator==(const Coord& rhs) const {
        return col == rhs.col && ln == rhs.ln;
    }

    friend std::ostream& operator<<(std::ostream& os, const Coord& self) {
        return os << "{ Ln " << self.ln << ", Col " << self.col << " }";
    }
};

namespace std {
template<>
struct hash<Coord> {
    size_t operator()(const Coord& coord) const {
        return std::hash<size_t>()(coord.col) ^ (std::hash<size_t>()(coord.ln) << 1);
    }
};
}  // namespace std

struct Bounds {
    size_t x1{};
    size_t x2{};
    size_t y1{};
    size_t y2{};

    [[nodiscard]]
    bool contains(const Coord& coord) const {
        return coord.col >= x1 && coord.col < x2 && coord.ln >= y1 && coord.ln < y2;
    }
};

struct Span {
    size_t line{};
    size_t start{};
    size_t count{};

    [[nodiscard]]
    bool is_adjacent(const Coord& coord) const {
        // get the adjacency bounding box
        auto x1 = start;
        if (x1 > 0) {
            x1--;
        }
        auto x2 = start + count + 1;
        auto y1 = line;
        if (y1 > 0) {
            y1--;
        }
        auto y2 = line + 2;
        Bounds b{x1, x2, y1, y2};

        return b.contains(coord);
    }
};

class Schematic {
public:
    friend std::ostream;

    static Schematic parse(const std::filesystem::path& path) {
        auto input = aoc::open(path);

        Schematic schematic{};
        std::string line{};

        // read all rows to build grid and get boundaries
        while (std::getline(input, line)) {
            std::vector<char> row{line.begin(), line.end()};

            auto xmax = row.size();
            if (!schematic._xmax) {
                schematic._xmax = xmax;
            } else if (schematic._xmax != xmax) {
                aoc::panic("Inconsistent schematic width");
            }

            schematic._grid.push_back(row);
        }
        AOC_ASSERT(!schematic._grid.empty(), "Schematic contains no rows");

        schematic._ymax = schematic._grid.size();
        schematic.process_grid();
        schematic.cache_gears();

        return schematic;
    }

    [[nodiscard]]
    const std::vector<Span>& number_spans() const {
        return _numbers;
    }

    [[nodiscard]]
    const std::unordered_map<Coord, std::pair<Span, Span>>& gears() const {
        return _gears;
    }

    [[nodiscard]]
    bool has_symbol(const Span& span) const {
        const auto bounds = get_bounds(span);
        for (auto j = bounds.y1; j < bounds.y2; ++j) {
            for (auto i = bounds.x1; i < bounds.x2; ++i) {
                if (_symbols.contains({i, j})) {
                    return true;
                }
            }
        }

        return false;
    }

    [[nodiscard]]
    int parse_span(const Span& span) const {
        const auto& row = _grid.at(span.line);
        using diff_t = std::vector<Span>::difference_type;
        const auto begin = row.begin() + static_cast<diff_t>(span.start);
        const auto end = begin + static_cast<diff_t>(span.count);
        const auto numstr = std::string{begin, end};

        return std::stoi(numstr);
    }

private:
    void process_grid() {
        size_t ln{};
        std::optional<Span> s{};

        // simple lambda to "finish" a span and clear it
        auto pop_s = [&s, this](size_t col) {
            if (s.has_value()) {
                if (col <= s->start) {
                    aoc::panic("Span count cannot be negative");
                }
                s->count = col - s->start;
                this->_numbers.push_back(s.value());
                s.reset();
            }
        };

        for (const auto& row : _grid) {
            // track numbers and symbols inserted to each row
            for (size_t i = 0; i < row.size(); ++i) {
                const auto c = row.at(i);

                if (std::isdigit(c)) {
                    if (!s.has_value()) {
                        s = Span{ln, i, 0};
                    }
                    continue;
                }

                if (!std::isdigit(c)) {
                    pop_s(i);
                }
                if (c != '.') {
                    Coord coord{i, ln};
                    _symbols[coord] = c;
                }
            }

            // spans can't cross lines, so store the current one if started
            if (s.has_value()) {
                pop_s(_xmax);
            }
            ln++;
        }
    }

    void cache_gears() {
        for (const auto& [coord, c] : _symbols) {
            if (c != '*') {
                continue;
            }
            std::vector<Span> adjacent{};

            // limit search to lines that are adjacent (above, on, below)
            auto min_ln = coord.ln;
            if (min_ln > 0) {
                min_ln--;
            }
            auto max_ln = coord.ln + 2;

            for (const auto& span : _numbers) {
                if (span.line < min_ln || span.line >= max_ln) {
                    continue;
                }
                if (span.is_adjacent(coord)) {
                    adjacent.push_back(span);
                }
                if (adjacent.size() > 2) {
                    break;
                }
            }

            if (adjacent.size() == 2) {
                _gears[coord] = {adjacent[0], adjacent[1]};
            }
        }
    }

    [[nodiscard]]
    Bounds get_bounds(const Span& span) const {
        auto x1 = span.start;
        if (x1 > 0) {
            x1--;
        }
        auto x2 = std::min(_xmax, span.start + span.count + 1);
        auto y1 = span.line;
        if (y1 > 0) {
            y1--;
        }
        auto y2 = std::min(_ymax, span.line + 2);

        return {x1, x2, y1, y2};
    }

    size_t _xmax{};
    size_t _ymax{};
    std::vector<std::vector<char>> _grid{};
    std::vector<Span> _numbers{};
    std::unordered_map<Coord, char> _symbols{};
    std::unordered_map<Coord, std::pair<Span, Span>> _gears{};
};

void part1(const Schematic& schematic) {
    int sum{};

    for (const auto& s : schematic.number_spans()) {
        if (schematic.has_symbol(s)) {
            sum += schematic.parse_span(s);
        }
    }
    std::cout << "Part 1: " << sum << '\n';
}

void part2(const Schematic& schematic) {
    size_t sum{};

    for (const auto& [coord, parts] : schematic.gears()) {
        auto first = static_cast<size_t>(schematic.parse_span(parts.first));
        auto second = static_cast<size_t>(schematic.parse_span(parts.second));
        sum += first * second;
    }

    std::cout << "Part 2: " << sum << '\n';
}

int main() noexcept(false) {
    auto schematic = Schematic::parse("data/day3.txt");
    part1(schematic);
    part2(schematic);
}
