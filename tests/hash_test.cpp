#include "aoc/hash.hpp"

#include <gtest/gtest.h>

#include <string>
#include <unordered_set>

struct Node {
    std::string label{};
    int value{};

    auto operator<=>(const Node& other) const = default;
};

const Node TEST_NODE{"foo", -3};

TEST(AocHash, HashCombine) {
    size_t s1{};
    aoc::hash_combine(s1, TEST_NODE.label);
    size_t s2{};
    aoc::hash_combine(s2, std::string{"foo"});

    EXPECT_EQ(s1, s2);
}

TEST(AocHash, HashCombineVArgs) {
    Node node{"foo", 42};

    size_t s1{};
    aoc::hash_combine(s1, TEST_NODE.label, TEST_NODE.value);
    size_t s2{};
    aoc::hash_combine(s2, node.label, node.value);

    EXPECT_NE(s1, s2);

    node.value = TEST_NODE.value;
    s2 = 0;
    aoc::hash_combine(s2, node.label, node.value);

    EXPECT_EQ(s1, s2);
}

namespace std {

template<>
struct hash<Node> {
    size_t operator()(const Node& node) const {
        size_t seed{0};
        aoc::hash_combine(seed, node.label, node.value);
        return seed;
    }
};

}  // namespace std

TEST(AocHash, HashStructStdAdl) {
    Node node{"foo", 42};
    std::unordered_set<Node> nodes{};
    nodes.insert(node);

    EXPECT_TRUE(nodes.contains(node));
    EXPECT_FALSE(nodes.contains(TEST_NODE));
    EXPECT_NE(std::hash<Node>{}(TEST_NODE), std::hash<Node>{}(node));

    nodes.clear();
    node.value = TEST_NODE.value;
    nodes.insert(node);

    EXPECT_EQ(std::hash<Node>{}(TEST_NODE), std::hash<Node>{}(node));
    EXPECT_TRUE(nodes.contains(node));
    EXPECT_TRUE(nodes.contains(TEST_NODE));
}
