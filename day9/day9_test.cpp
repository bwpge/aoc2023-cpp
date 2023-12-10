#include "day9.hpp"

#include <gtest/gtest.h>

using day9::Reading;
using Direction = Reading::Direction;

TEST(Day9, Reading) {
    const std::vector<std::string> lines{
        "0 3 6 9 12 15",
        "1 3 6 10 15 21",
        "10 13 16 21 30 45",
    };
    const std::vector<std::deque<int64_t>> values{
        {0, 3, 6, 9, 12, 15},
        {1, 3, 6, 10, 15, 21},
        {10, 13, 16, 21, 30, 45},
    };

    for (auto&& [line, expected] : aoc::zip_copy(lines, values)) {
        auto r = Reading::parse(line);
        EXPECT_EQ(r.values(), expected);
    }
}

TEST(Day9, ReadingFuture) {
    const std::vector<std::pair<std::string, int64_t>> test_data{
        {"0 3 6 9 12 15", 18},
        {"1 3 6 10 15 21", 28},
        {"10 13 16 21 30 45", 68},
    };

    for (const auto& [line, expected] : test_data) {
        auto r = Reading::parse(line);
        EXPECT_EQ(r.analyze(Direction::Future), expected);
    }
}

TEST(Day9, ReadingPast) {

    const std::vector<std::pair<std::string, int64_t>> test_data{
        {"0 3 6 9 12 15", -3},
        {"1 3 6 10 15 21", 0},
        {"10 13 16 21 30 45", 5},
    };

    for (const auto& [line, expected] : test_data) {
        auto r = Reading::parse(line);
        EXPECT_EQ(r.analyze(Direction::Past), expected);
    }
}
