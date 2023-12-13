#pragma once

#include "aoc/aoc.hpp"

#include <unordered_set>

namespace std {

template<>
struct hash<std::pair<aoc::Coord, aoc::Coord>> {
    size_t operator()(const std::pair<aoc::Coord, aoc::Coord>& pair) const {
        size_t seed{};
        aoc::hash_combine(seed, pair.first, pair.second);

        return seed;
    }
};

}  // namespace std

namespace day11 {

using aoc::Coord;
using aoc::Grid;
using aoc::range_val;

inline bool crosses_row(size_t row, const Coord& from, const Coord& to) {
    auto lo = std::min(from.y, to.y);
    auto hi = std::max(from.y, to.y);
    return lo < row && row < hi;
}

inline bool crosses_col(size_t col, const Coord& from, const Coord& to) {
    auto lo = std::min(from.x, to.x);
    auto hi = std::max(from.x, to.x);
    return lo < col && col < hi;
}

class GalaxyMap : public Grid<char> {
public:
    friend GalaxyMap make_galaxy_map(const std::vector<std::string>&);

    explicit GalaxyMap(aoc::Grid<char>&& other) : aoc::Grid<char>(std::move(other)) {
        cache_galaxy_coords();
        find_expansion_space();
    }

    [[nodiscard]]
    size_t min_distance(const Coord& from, const Coord& to) const override {
        auto distance = Grid<char>::min_distance(from, to);
        if (_expansion_factor == 0) {
            return distance;
        }

        auto factor = _expansion_factor == 1 ? 1 : _expansion_factor - 1;

        size_t count{};
        count += std::ranges::count_if(_expand_rows, [&from, &to](auto row) {
            return crosses_row(row, from, to);
        });
        count += std::ranges::count_if(_expand_cols, [&from, &to](auto col) {
            return crosses_col(col, from, to);
        });

        return distance + (count * factor);
    }

    [[nodiscard]]
    std::unordered_set<std::pair<Coord, Coord>> galaxy_pairs() const {
        std::unordered_set<std::pair<Coord, Coord>> pairs{};
        for (size_t i = 0; i < _galaxies.size(); ++i) {
            for (size_t j = i + 1; j < _galaxies.size(); ++j) {
                pairs.insert({_galaxies[i], _galaxies[j]});
            }
        }

        return pairs;
    }

    [[nodiscard]]
    const std::vector<Coord>& galaxy_coords() const {
        return _galaxies;
    }

    [[nodiscard]]
    size_t expansion_factor() const {
        return _expansion_factor;
    }

    void set_expansion_factor(size_t expansion_factor) {
        _expansion_factor = expansion_factor;
    }

private:
    void cache_galaxy_coords() {
        _galaxies.clear();
        for (size_t x = 0; x < width(); ++x) {
            for (size_t y = 0; y < height(); ++y) {
                if (at(x, y) == '#') {
                    _galaxies.push_back({x, y});
                }
            }
        }
    }

    void find_expansion_space() {
        _expand_rows.clear();
        _expand_cols.clear();

        // find rows to expand
        for (size_t y = 0; y < height(); ++y) {
            bool empty{true};
            for (size_t x = 0; x < width(); ++x) {
                empty &= at(x, y) == '.';
            }
            if (empty) {
                _expand_rows.insert(y);
            }
        }

        // find columns to expand
        for (size_t x = 0; x < width(); ++x) {
            bool empty{true};
            for (size_t y = 0; y < height(); ++y) {
                empty &= at(x, y) == '.';
            }
            if (empty) {
                _expand_cols.insert(x);
            }
        }
    }

    std::vector<Coord> _galaxies{};
    size_t _expansion_factor{};
    std::unordered_set<size_t> _expand_cols{};
    std::unordered_set<size_t> _expand_rows{};
};

inline GalaxyMap make_galaxy_map(
    const std::vector<std::string>& data,
    size_t expansion_factor = 0
) {
    auto g = GalaxyMap{aoc::make_grid(data)};
    g.set_expansion_factor(expansion_factor);
    return g;
}

}  // namespace day11
