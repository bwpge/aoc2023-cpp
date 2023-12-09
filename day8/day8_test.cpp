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

TEST(Day8, StepsParallel) {
    Network network{};
    network.add({"11A", "11B", "XXX"});
    network.add({"11B", "XXX", "11Z"});
    network.add({"11Z", "11B", "XXX"});
    network.add({"22A", "22B", "XXX"});
    network.add({"22B", "22C", "22C"});
    network.add({"22C", "22Z", "22Z"});
    network.add({"22Z", "22B", "22B"});
    network.add({"XXX", "XXX", "XXX"});

    auto steps = network.steps_parallel("LR");
    EXPECT_EQ(steps, 6);
}
