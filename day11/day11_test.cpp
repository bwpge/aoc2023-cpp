#include "day11.hpp"

#include <gtest/gtest.h>

using aoc::Coord;

const std::vector<std::string> MAP_DATA{
    "...#......",
    ".......#..",
    "#.........",
    "..........",
    "......#...",
    ".#........",
    ".........#",
    "..........",
    ".......#..",
    "#...#.....",
};

TEST(Day11, MakeGalaxyMap) {
    auto g = day11::make_galaxy_map(MAP_DATA);
    EXPECT_EQ(g.width(), MAP_DATA.front().size());
    EXPECT_EQ(g.height(), MAP_DATA.size());

    for (size_t j = 0; j < MAP_DATA.size(); j++) {
        for (size_t i = 0; i < MAP_DATA.front().size(); ++i) {
            // note i/j are flipped on vector to compare with row major grid access
            EXPECT_EQ(g.at(i, j), MAP_DATA[j][i]) << "at i=" << i << ", j=" << j;
        }
    }
}

TEST(Day11, GalaxyMapExpansion) {
    auto g = day11::make_galaxy_map(MAP_DATA);
    EXPECT_EQ(g.expansion_factor(), 0);
    EXPECT_EQ(g.width(), MAP_DATA.front().size());
    EXPECT_EQ(g.height(), MAP_DATA.size());

    std::vector<std::pair<size_t, size_t>> pairs{
        {0, 7},
        {1, 9},
        {1000000, 2000005},
    };

    for (const auto& [factor, expected] : pairs) {
        g.set_expansion_factor(factor);
        EXPECT_EQ(g.min_distance({1, 5}, {4, 9}), expected) << "with expansion factor " << factor;
    }
}

TEST(Day11, GalaxyMapCoords) {
    auto g = day11::make_galaxy_map(MAP_DATA);
    const std::vector<Coord> expected_coords{
        {0, 2},
        {0, 9},
        {3, 0},
        {1, 5},
        {6, 4},
        {4, 9},
        {9, 6},
        {7, 1},
        {7, 8},
    };

    EXPECT_EQ(g.galaxy_coords().size(), 9);

    auto galaxy_coords = g.galaxy_coords();
    for (const auto& expected : expected_coords) {
        // note x/y are flipped on vector to compare with row major grid access
        EXPECT_EQ(g.at(expected), MAP_DATA[expected.y][expected.x]);
        EXPECT_TRUE(aoc::contains(expected_coords, expected)
        ) << "expected ("
          << expected.x << ", " << expected.y << ") to be in galaxy coordinates";
    }
}

TEST(Day11, GalaxyMapPairs) {
    auto g = day11::make_galaxy_map(MAP_DATA);
    auto pairs = g.galaxy_pairs();

    EXPECT_EQ(pairs.size(), 36);
    for (const auto& [c1, c2] : pairs) {
        EXPECT_FALSE(pairs.contains({c2, c1})
        ) << "galaxy pairs should not contain swapped coordinates";
    }
}

TEST(Day11, GalaxyMapPairMinDistance) {
    auto g = day11::make_galaxy_map(MAP_DATA);

    std::vector<std::pair<size_t, size_t>> pairs{
        {1, 374},
        {10, 1030},
        {100, 8410},
    };

    for (const auto& [factor, expected] : pairs) {
        g.set_expansion_factor(factor);
        size_t sum{};

        for (const auto& [from, to] : g.galaxy_pairs()) {
            sum += g.min_distance(from, to);
        }

        EXPECT_EQ(sum, expected) << "with expansion factor " << factor;
    }
}
