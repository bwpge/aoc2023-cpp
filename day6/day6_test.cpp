#include "day6.hpp"

#include <gtest/gtest.h>

TEST(Day6, RaceCanWin) {
    day6::Race race{7, 9};

    for (size_t i = 0; i < 2; ++i) {
        EXPECT_FALSE(race.can_win(i)) << "charge_ms=" << i;
    }
    for (size_t i = 2; i < 6; ++i) {
        EXPECT_TRUE(race.can_win(i)) << "charge_ms=" << i;
    }
    for (size_t i = 6; i < 9; ++i) {
        EXPECT_FALSE(race.can_win(i)) << "charge_ms=" << i;
    }
}

TEST(Day6, RaceWinCondition) {
    day6::Race race{7, 9};
    auto [t_min, t_max] = race.win_condition();

    EXPECT_EQ(t_min, 2);
    EXPECT_EQ(t_max, 5);
}

const auto EXAMPLE_RACES = std::vector<std::tuple<day6::Race, size_t, size_t>>{
    {day6::Race{7, 9}, 2, 5},
    {day6::Race{15, 40}, 4, 11},
    {day6::Race{30, 200}, 11, 19},
};

TEST(Day6, ExampleWinConditions) {
    std::vector<day6::Race> races{};
    for (const auto& [race, expected_min, expected_max] : EXAMPLE_RACES) {
        auto&& [t_min, t_max] = race.win_condition();
        EXPECT_EQ(t_min, expected_min);
        EXPECT_EQ(t_max, expected_max);

        races.push_back(race);
    }

    EXPECT_EQ(day6::margin_of_error(races), 288);
}
