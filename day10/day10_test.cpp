#include "day10.hpp"

#include <gtest/gtest.h>

#include <ranges>

using day10::Coord;
using day10::Maze;
using Direction = Maze::Direction;

const std::vector<std::string> SIMPLE_PIPES{
    ".....",
    ".S-7.",
    ".|.|.",
    ".L-J.",
    ".....",
};

const std::vector<std::string> PIPES{
    "-L|F7",
    "7S-7|",
    "L|7||",
    "-L-J|",
    "L|-JF",
};

TEST(Day10, Maze) {
    auto m = Maze::parse(SIMPLE_PIPES);

    EXPECT_EQ(m.width(), 5);
    EXPECT_EQ(m.height(), 5);
}

TEST(Day10, MazeConnects) {
    // has the form: [ coord, expected=(north, east, south, west) ]
    const std::vector<std::pair<Coord, std::tuple<bool, bool, bool, bool>>> test_data{
        {{0, 0}, {false, false, false, false}},  // .
        {{1, 1}, {true, true, true, true}},  // S
        {{2, 1}, {false, true, false, true}},  // -
        {{3, 1}, {false, false, true, true}},  // 7
        {{1, 2}, {true, false, true, false}},  // |
        {{3, 2}, {true, false, true, false}},  // |
        {{1, 3}, {true, true, false, false}},  // L
        {{2, 3}, {false, true, false, true}},  // -
        {{3, 3}, {true, false, false, true}},  // J
    };

    auto m = Maze::parse(SIMPLE_PIPES);
    for (const auto& [pos, expected] : test_data) {
        auto [expect_n, expect_e, expect_s, expect_w] = expected;
        auto msg = fmt::format("at pos=({}, {})", pos.x, pos.y);

        EXPECT_EQ(m.connects_to(pos, Direction::North), expect_n) << msg;
        EXPECT_EQ(m.connects_to(pos, Direction::East), expect_e) << msg;
        EXPECT_EQ(m.connects_to(pos, Direction::South), expect_s) << msg;
        EXPECT_EQ(m.connects_to(pos, Direction::West), expect_w) << msg;
    }
}

TEST(Day10, MazeConnectsTo) {
    auto m = Maze::parse(PIPES);

    // test incoming connections to start
    Coord start{1, 1};
    Coord north{1, 0};
    Coord east{2, 1};
    Coord south{1, 2};
    Coord west{0, 1};
    EXPECT_FALSE(m.connects_to(north, start));
    EXPECT_TRUE(m.connects_to(east, start));
    EXPECT_TRUE(m.connects_to(south, start));
    EXPECT_FALSE(m.connects_to(west, start));

    // test some arbitrary tiles
    EXPECT_FALSE(m.connects_to({0, 0}, {0, 1}));  // - to 7, S
    EXPECT_FALSE(m.connects_to({2, 4}, {2, 3}));  // - to -, N
    EXPECT_TRUE(m.connects_to({3, 3}, {2, 3}));  // J to -, W
    EXPECT_TRUE(m.connects_to({1, 3}, {2, 3}));  // L to -, E
}

TEST(Day10, MazeAdjacent) {
    auto m = Maze::parse(PIPES);
    std::vector<std::pair<Coord, std::vector<Coord>>> expect{
        {
            {1, 1},
            {{1, 2}, {2, 1}},
        },
        {{2, 1}, {{1, 1}, {3, 1}}},
        {{2, 2}, {}},
        {{3, 3}, {{3, 2}, {2, 3}}}
    };

    for (const auto& [src, coords] : expect) {
        auto adjacent = m.adjacent(src);
        auto msg_size = fmt::format("size mistmatch for src=({}, {})", src.x, src.y);
        EXPECT_EQ(adjacent.size(), coords.size()) << msg_size;

        for (const auto& expected : coords) {
            auto msg = fmt::format(
                "missing expected neighbor ({}, {}) for src=({}, {})",
                expected.x,
                expected.y,
                src.x,
                src.y
            );
            EXPECT_TRUE(aoc::contains(adjacent, expected)) << msg;
        }
    }
}

TEST(Day10, MazeFurthest) {
    const size_t expected = 4;
    auto m = Maze::parse(PIPES);
    EXPECT_EQ(m.furthest(), expected);
}

TEST(Day10, MazeFurthestComplex) {
    const size_t expected = 8;
    const std::vector<std::string> data{
        "..F7.",
        ".FJ|.",
        "SJ.L7",
        "|F--J",
        "LJ...",
    };
    auto m = Maze::parse(data);
    EXPECT_EQ(m.furthest(), expected);
}
