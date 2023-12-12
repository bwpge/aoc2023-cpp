#include "aoc/aoc.hpp"

#include <gtest/gtest.h>

using aoc::Coord;

const std::vector<std::string> EMPTY_MAP{
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
};

TEST(AocGrid, MakeGrid) {
    auto grid = aoc::make_grid(EMPTY_MAP);
    EXPECT_EQ(grid.width(), 8);
    EXPECT_EQ(grid.height(), 10);

    grid.at(1, 1) = '#';
    EXPECT_EQ(grid.at(0, 1), '.');
    EXPECT_EQ(grid.at(1, 1), '#');
}

TEST(AocGrid, MakeGridStruct) {
    struct Node {
        std::string label{};
        int value{};
        auto operator<=>(const Node& other) const = default;
    };

    const Node n1{"foo", 1};
    const Node n2{"bar", -999999999};
    const Node n3{"baz", 51293786};
    const Node n4{"qux", -3};
    const std::vector<std::vector<Node>> nodes{
        {n1, n2},
        {n3, n4},
    };

    auto grid = aoc::make_grid(nodes);
    EXPECT_EQ(grid.width(), 2);
    EXPECT_EQ(grid.height(), 2);
    EXPECT_EQ(grid.at(0, 0), n1);
    EXPECT_EQ(grid.at(1, 0), n2);
    EXPECT_EQ(grid.at(0, 1), n3);
    EXPECT_EQ(grid.at(1, 1), n4);
}

TEST(AocGrid, InsertRow) {
    auto grid = aoc::make_grid(EMPTY_MAP);
    const size_t row = 2;

    grid.insert_row(row, '#');
    for (size_t i = 0; i < EMPTY_MAP.front().size(); ++i) {
        for (size_t j = 0; j < EMPTY_MAP.size(); ++j) {
            if (j == row) {
                EXPECT_EQ(grid.at(i, j), '#') << "at (" << i << ", " << j << ")";
            } else {
                EXPECT_EQ(grid.at(i, j), '.') << "at (" << i << ", " << j << ")";
            }
        }
    }
}

TEST(AocGrid, InsertColumn) {
    auto grid = aoc::make_grid(EMPTY_MAP);
    const size_t col = 5;

    grid.insert_col(col, '#');
    for (size_t i = 0; i < EMPTY_MAP.front().size(); ++i) {
        for (size_t j = 0; j < EMPTY_MAP.size(); ++j) {
            if (i == col) {
                EXPECT_EQ(grid.at(i, j), '#') << "at (" << i << ", " << j << ")";
            } else {
                EXPECT_EQ(grid.at(i, j), '.') << "at (" << i << ", " << j << ")";
            }
        }
    }
}

TEST(AocGrid, Adjacent) {
    // has the form: [ coord, number adjacent, (adjacent coords) ]
    const std::vector<std::tuple<Coord, size_t, std::vector<Coord>>> test_data{
        {{0, 0}, 2, {{0, 1}, {1, 0}}},
        {{1, 6}, 4, {{1, 5}, {1, 7}, {0, 6}, {2, 6}}},
        {{7, 5}, 3, {{6, 5}, {7, 4}, {7, 6}}},
    };
    auto grid = aoc::make_grid(EMPTY_MAP);

    for (const auto& [pos, count, coords] : test_data) {
        auto adjacent = grid.adjacent(pos);
        EXPECT_EQ(adjacent.size(), count) << "with pos=(" << pos.x << ", " << pos.y << ")";

        for (const auto& coord : coords) {
            auto msg = fmt::format(
                "expected ({}, {}) to be adjacent to ({}, {})",
                coord.x,
                coord.y,
                pos.x,
                pos.y
            );
            EXPECT_TRUE(aoc::contains(adjacent, coord)) << msg;
        }
    }
}

TEST(AocGrid, MinDistance) {
    auto grid = aoc::make_grid(EMPTY_MAP);
    Coord p1{0, 0};
    Coord p2{1, 6};
    Coord p3{7, 5};

    EXPECT_EQ(grid.min_distance(p1, p1), 0);
    EXPECT_EQ(grid.min_distance(p1, p2), 7);
    EXPECT_EQ(grid.min_distance(p1, p3), 12);
    EXPECT_EQ(grid.min_distance(p3, p2), 7);
}

TEST(AocGrid, Derived) {
    class DerivedGrid : public aoc::Grid<char> {
    public:
        explicit DerivedGrid(aoc::Grid<char>&& other) : aoc::Grid<char>(std::move(other)) {}
    };

    DerivedGrid grid{aoc::make_grid(EMPTY_MAP)};
    EXPECT_EQ(grid.at(1, 1), '.');
}
