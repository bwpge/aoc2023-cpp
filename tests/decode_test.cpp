#include "aoc/digit.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>

// Has the form (INPUT, LEFT_DIGIT, RIGHT_DIGIT)
const std::vector<std::tuple<std::string, int, int>> TEST_DATA{
    {"1abc2", 1, 2},
    {"pqr3stu8vwx", 3, 8},
    {"a1b2c3d4e5f", 1, 5},
    {"treb7uchet", 7, 7},
    {"two1nine", 2, 9},
    {"eightwothree", 8, 3},
    {"abcone2threexyz", 1, 3},
    {"xtwone3four", 2, 4},
    {"4nineeightseven2", 4, 2},
    {"zoneight234", 1, 4},
    {"7pqrstsixteen", 7, 6},
};

TEST(AocDigits, DecodeDigitsLeft) {
    for (const auto& [input, expected, _] : TEST_DATA) {
        std::optional<int> x{};

        for (size_t i = 0; i < input.length(); i++) {
            x = aoc::decode_digit(input, i);
            if (x.has_value()) {
                break;
            }
        }

        ASSERT_TRUE(x.has_value());
        EXPECT_EQ(x.value(), expected);
    }
}

TEST(AocDigits, DecodeDigitsRight) {
    for (const auto& [input, _, expected] : TEST_DATA) {
        std::optional<int> x{};

        for (size_t i = input.length() - 1; i >= 0; i--) {
            x = aoc::decode_digit(input, i);
            if (x.has_value()) {
                break;
            }
        }

        ASSERT_TRUE(x.has_value());
        EXPECT_EQ(x.value(), expected);
    }
}
