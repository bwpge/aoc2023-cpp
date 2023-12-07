#include "aoc/collections.hpp"

#include <gtest/gtest.h>

TEST(AocCollections, ZipCopy) {
    std::vector<int> a{1, 2, 3, 4};
    std::set<std::string> b{"foo", "bar", "baz"};

    std::vector<std::pair<int, std::string>> expected{
        {1, "foo"},
        {2, "bar"},
        {3, "baz"},
    };
    auto result = aoc::zip_copy(a, b);

    for (size_t i = 0; i < expected.size(); ++i) {
        const auto [first, second] = result.at(i);
        const auto [exp_first, exp_second] = expected.at(i);

        EXPECT_EQ(first, exp_first) << "Mismatch on first at i=" << i;
        EXPECT_EQ(second, exp_second) << "Mismatch on second at i=" << i;
    }
}
