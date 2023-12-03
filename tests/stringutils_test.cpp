#include "aoc/stringutils.hpp"

#include <gtest/gtest.h>

const std::vector<std::tuple<std::string, std::string>> TEST_DATA_LOWER{
    {"", ""},
    {" ", " "},
    {"abc123", "abc123"},
    {"xaB)_t+!@#()x", "xab)_t+!@#()x"},
};

const std::vector<std::tuple<std::string, std::string>> TEST_DATA_UPPER{
    {"", ""},
    {" ", " "},
    {"abc123", "ABC123"},
    {"xaB)_t+!@#()x", "XAB)_T+!@#()X"},
};

TEST(AocLib, StringToLower) {
    for (const auto& [input, expected] : TEST_DATA_LOWER) {
        std::string value{input};
        aoc::to_lower(value);
        ASSERT_EQ(value, expected);
    }
}

TEST(AocLib, StringToLowerCopy) {
    for (const auto& [input, expected] : TEST_DATA_LOWER) {
        const auto result = aoc::to_lower_copy(input);
        ASSERT_EQ(result, expected);
    }
}

TEST(AocLib, StringToUpper) {

    for (const auto& [input, expected] : TEST_DATA_UPPER) {
        std::string value{input};
        aoc::to_upper(value);
        ASSERT_EQ(value, expected);
    }
}

TEST(AocLib, StringToUpperCopy) {
    for (const auto& [input, expected] : TEST_DATA_UPPER) {
        const auto result = aoc::to_upper_copy(input);
        ASSERT_EQ(result, expected);
    }
}
