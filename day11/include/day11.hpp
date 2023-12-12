#pragma once

#include "aoc/aoc.hpp"

#include <unordered_set>

namespace day11 {

using aoc::Coord;
using aoc::Grid;
using aoc::range_val;

class GalaxyMap : public Grid<char> {
public:
    friend GalaxyMap make_galaxy_map(const std::vector<std::string>&);

    explicit GalaxyMap(aoc::Grid<char>&& other) : aoc::Grid<char>(std::move(other)) {
        cache_galaxy_coords();
    }

    void expand() {
        std::vector<size_t> rows{};
        std::vector<size_t> cols{};

        // find rows to expand
        for (size_t y = 0; y < height(); ++y) {
            bool empty{true};
            for (size_t x = 0; x < width(); ++x) {
                empty &= at(x, y) == '.';
            }
            if (empty) {
                rows.push_back(y);
            }
        }

        // find columns to expand
        for (size_t x = 0; x < width(); ++x) {
            bool empty{true};
            for (size_t y = 0; y < height(); ++y) {
                empty &= at(x, y) == '.';
            }
            if (empty) {
                cols.push_back(x);
            }
        }

        // rows and columns need to be inserted in reverse order to make sure we are
        // expanding the correct ones without having to account for previous insertions
        while (!rows.empty()) {
            auto row = rows.back();
            rows.pop_back();
            insert_row(row, '.');
        }
        while (!cols.empty()) {
            auto col = cols.back();
            cols.pop_back();
            insert_col(col, '.');
        }

        // update galaxy coordinates
        cache_galaxy_coords();
    }

    [[nodiscard]]
    const std::unordered_set<Coord>& galaxy_coords() const {
        return _galaxies;
    }

private:
    void cache_galaxy_coords() {
        _galaxies.clear();
        for (size_t x = 0; x < width(); ++x) {
            for (size_t y = 0; y < height(); ++y) {
                if (at(x, y) == '#') {
                    _galaxies.insert({x, y});
                }
            }
        }
    }

    std::unordered_set<Coord> _galaxies{};
};

inline GalaxyMap make_galaxy_map(const std::vector<std::string>& data) {
    return GalaxyMap{aoc::make_grid(data)};
}

}  // namespace day11
