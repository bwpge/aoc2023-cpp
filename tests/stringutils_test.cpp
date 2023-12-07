#include "aoc/aoc.hpp"

#include <gtest/gtest.h>

#include <optional>

const std::vector<std::pair<std::string, std::string>> TEST_DATA_LOWER{
    {"", ""},
    {" ", " "},
    {"abc123", "abc123"},
    {"xaB)_t+!@#()x", "xab)_t+!@#()x"},
};

const std::vector<std::pair<std::string, std::string>> TEST_DATA_UPPER{
    {"", ""},
    {" ", " "},
    {"abc123", "ABC123"},
    {"xaB)_t+!@#()x", "XAB)_T+!@#()X"},
};

TEST(AocStringUtils, ToLower) {
    for (const auto& [input, expected] : TEST_DATA_LOWER) {
        std::string value{input};
        aoc::to_lower(value);
        ASSERT_EQ(value, expected);
    }
}

TEST(AocStringUtils, ToLowerCopy) {
    for (const auto& [input, expected] : TEST_DATA_LOWER) {
        const auto result = aoc::to_lower_copy(input);
        ASSERT_EQ(result, expected);
    }
}

TEST(AocStringUtils, ToUpper) {
    for (const auto& [input, expected] : TEST_DATA_UPPER) {
        std::string value{input};
        aoc::to_upper(value);
        ASSERT_EQ(value, expected);
    }
}

TEST(AocStringUtils, ToUpperCopy) {
    for (const auto& [input, expected] : TEST_DATA_UPPER) {
        const auto result = aoc::to_upper_copy(input);
        ASSERT_EQ(result, expected);
    }
}

TEST(AocStringUtils, SplitOnceChar) {
    const std::vector<std::tuple<std::string, char, std::string, std::string>> cases{
        {"ab:", ':', "ab", ""},
        {"a b", ' ', "a", "b"},
        {"a b c d", ' ', "a", "b c d"},
        {"xxxx", 'x', "", "xxx"},
        {"\t\n ", '\n', "\t", " "},
    };

    for (const auto& [input, delim, expected_l, expected_r] : cases) {
        const auto splits = aoc::split_once(input, delim);
        ASSERT_TRUE(splits.has_value());
        ASSERT_EQ(splits->first, expected_l);
        ASSERT_EQ(splits->second, expected_r);
    }

    const std::vector<std::pair<std::string, char>> no_split_data{
        {"abcdef", 'g'},
        {"ahj123 l;kj", '@'},
        {"198-2731oijdasl asldk;ja \n asdjklh89", '\t'},
    };
    for (const auto& [input, delim] : no_split_data) {
        const auto splits = aoc::split_once(input, delim);
        ASSERT_FALSE(splits.has_value());
    }
}

TEST(AocStringUtils, SplitOnceStr) {
    const std::vector<std::tuple<std::string, std::string, std::string, std::string>> cases{
        {"ab:", ":", "ab", ""},
        {"a b", " ", "a", "b"},
        {"a b c d", " b ", "a", "c d"},
        {"xx||xx", "||", "xx", "xx"},
        {"xxxx", "xx", "", "xx"},
        {"\t\n ", "\t\n", "", " "},
    };

    for (const auto& [input, delim, expected_l, expected_r] : cases) {
        const auto splits = aoc::split_once(input, delim);
        ASSERT_TRUE(splits.has_value()) << "Split should return a value";
        EXPECT_EQ(splits->first, expected_l);
        EXPECT_EQ(splits->second, expected_r);
    }

    const std::vector<std::pair<std::string, std::string>> no_split_data{
        {"abcdef", "xyz"},
        {"ahj123 l;kj", "!@#"},
        {"198-2731oijdasl asldk;ja \n asdjklh89", "\t"},
    };
    for (const auto& [input, delim] : no_split_data) {
        const auto splits = aoc::split_once(input, delim);
        ASSERT_FALSE(splits.has_value());
    }
}

TEST(AocStringUtils, SplitChar) {
    const std::vector<
        std::tuple<std::string, char, aoc::SplitOptions, std::vector<std::string_view>>>
        cases{
            {"x1x3x4x", 'x', aoc::SplitOptions::None, {"", "1", "3", "4", ""}},
            {"x1x3x4 x",
             'x',
             aoc::SplitOptions::DiscardEmpty | aoc::SplitOptions::Trim,
             {"1", "3", "4"}},
            {"xxxx", 'x', aoc::SplitOptions::None, {"", "", "", "", ""}},
            {"abc", 'x', aoc::SplitOptions::None, {"abc"}},
            {"abc = 1!=", '=', aoc::SplitOptions::None, {"abc ", " 1!", ""}}
        };

    for (const auto& [input, delim, opts, expected] : cases) {
        const auto splits = aoc::split(input, delim, opts);

        ASSERT_EQ(
            splits.size(),
            expected.size()
        ) << "Split returned incorrect length with:\n  input = '"
          << input << "'\n  delimiter = '" << delim << "\n  expected = "
          << testing::PrintToString(expected) << "\n  actual = " << testing::PrintToString(splits);
        for (size_t i = 0; i < splits.size(); i++) {
            EXPECT_EQ(splits[i], expected[i]) << "Split differs at i = " << i;
        }
    }
}

struct TrimTestCase {
    std::string input{};

    struct {
        std::string left{};
        std::string right{};
        std::string both{};
    } expected{};
};

const std::vector<TrimTestCase> TEST_DATA_TRIM{
    {"foo \t \r\n", {"foo \t \r\n", "foo", "foo"}},
    {" \t \r\nfoo", {"foo", " \t \r\nfoo", "foo"}},
    {"  a  ", {"a  ", "  a", "a"}},
    {"\r\n\v  \t  ", {"", "", ""}},
    {"a\tb", {"a\tb", "a\tb", "a\tb"}},
    {"Lorem\tipsum  dolor   sit amet          ",
     {
         "Lorem\tipsum  dolor   sit amet          ",
         "Lorem\tipsum  dolor   sit amet",
         "Lorem\tipsum  dolor   sit amet",
     }},
};

TEST(AocStringUtils, TrimLeft) {
    for (const auto& [input, expected] : TEST_DATA_TRIM) {
        auto trimmed = aoc::trim_left(input);
        EXPECT_EQ(trimmed, expected.left);
    }
}

TEST(AocStringUtils, TrimRight) {
    for (const auto& [input, expected] : TEST_DATA_TRIM) {
        auto trimmed = aoc::trim_right(input);
        EXPECT_EQ(trimmed, expected.right);
    }
}

TEST(AocStringUtils, Trim) {
    for (const auto& [input, expected] : TEST_DATA_TRIM) {
        auto trimmed = aoc::trim(input);
        EXPECT_EQ(trimmed, expected.both);
    }
}
