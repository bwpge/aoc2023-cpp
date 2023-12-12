#include "day11.hpp"

#include <gtest/gtest.h>

using aoc::Coord;

const std::vector<std::string> EXAMPLE{
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

const std::vector<std::string> EXPANDED{
    "....#........",
    ".........#...",
    "#............",
    ".............",
    ".............",
    "........#....",
    ".#...........",
    "............#",
    ".............",
    ".............",
    ".........#...",
    "#....#.......",
};

TEST(Day11, MakeGalaxyMap) {
    auto g = day11::make_galaxy_map(EXAMPLE);
    EXPECT_EQ(g.width(), EXAMPLE.front().size());
    EXPECT_EQ(g.height(), EXAMPLE.size());

    for (size_t j = 0; j < EXAMPLE.size(); j++) {
        for (size_t i = 0; i < EXAMPLE.front().size(); ++i) {
            // note i/j are flipped on vector to compare with row major grid access
            EXPECT_EQ(g.at(i, j), EXAMPLE[j][i]) << "at i=" << i << ", j=" << j;
        }
    }
}

TEST(Day11, GalaxyMapExpand) {
    auto g = day11::make_galaxy_map(EXAMPLE);
    g.expand();
    EXPECT_EQ(g.width(), EXPANDED.front().size());
    EXPECT_EQ(g.height(), EXPANDED.size());

    for (size_t j = 0; j < EXPANDED.size(); j++) {
        for (size_t i = 0; i < EXPANDED.front().size(); ++i) {
            // note i/j are flipped on vector to compare with row major grid access
            EXPECT_EQ(g.at(i, j), EXPANDED[j][i]) << "at i=" << i << ", j=" << j;
        }
    }
}

TEST(Day11, GalaxyMapCoords) {
    auto g = day11::make_galaxy_map(EXAMPLE);
    const std::vector<Coord> galaxy_coords{
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
    const std::vector<Coord> expanded_coords{
        {0, 2},
        {0, 11},
        {1, 6},
        {4, 0},
        {5, 11},
        {8, 5},
        {9, 1},
        {9, 10},
        {12, 7},
    };

    EXPECT_EQ(g.galaxy_coords().size(), 9);
    for (const auto& expected : galaxy_coords) {
        EXPECT_TRUE(g.galaxy_coords().contains(expected)
        ) << "expected ("
          << expected.x << ", " << expected.y << ") to be in galaxy data";
    }

    g.expand();

    EXPECT_EQ(g.galaxy_coords().size(), 9);
    for (const auto& expected : expanded_coords) {
        EXPECT_TRUE(g.galaxy_coords().contains(expected)
        ) << "expected ("
          << expected.x << ", " << expected.y << ") to be in expanded galaxy data";
    }
}
