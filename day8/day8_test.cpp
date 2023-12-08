#include "day8.hpp"

#include <gtest/gtest.h>

using day8::Direction;
using day8::Network;

TEST(Day8, Direction) {
    auto dir = Direction::parse("AAA = (BBB, CCC)");
    EXPECT_EQ(dir.name, "AAA");
    EXPECT_EQ(dir.left, "BBB");
    EXPECT_EQ(dir.right, "CCC");
}

TEST(Day8, Steps) {
    Network network{};
    network.add({"AAA", "BBB", "BBB"});
    network.add({"BBB", "AAA", "ZZZ"});
    network.add({"ZZZ", "ZZZ", "ZZZ"});

    auto steps = network.steps("LLR");
    EXPECT_EQ(steps, 6);
}
