#include "day5.hpp"

#include <gtest/gtest.h>

TEST(Day5, Range) {
    day5::Range<uint32_t> range{50, 52, 48};

    EXPECT_TRUE(range.contains(79));
    EXPECT_FALSE(range.contains(98));
    EXPECT_FALSE(range.contains(49));
    EXPECT_EQ(range.offset(96), 98);
}

TEST(Day5, RangeRev) {
    day5::Range<uint32_t> range{50, 52, 48};

    EXPECT_TRUE(range.rcontains(79));
    EXPECT_TRUE(range.rcontains(98));
    EXPECT_FALSE(range.rcontains(49));
    EXPECT_EQ(range.roffset(98), 96);
}

TEST(Day5, RangeMap) {
    day5::RangeMap<uint32_t> map{{
        {98, 50, 2},
        {50, 52, 48},
    }};

    EXPECT_TRUE(map.contains(79));
    EXPECT_TRUE(map.contains(98));
    EXPECT_FALSE(map.contains(49));
    EXPECT_EQ(map.at(96), 98);
    EXPECT_EQ(map.at(98), 50);
    EXPECT_EQ(map.at(0), 0);
    EXPECT_EQ(map.at(49), 49);
}

day5::Almanac example() {
    day5::Almanac almanac{};
    almanac.set_seeds({79, 14, 55, 13});
    almanac["seed-to-soil"] = day5::RangeMap<size_t>{{
        {98, 50, 2},
        {50, 52, 48},
    }};
    almanac["soil-to-fertilizer"] = day5::RangeMap<size_t>{{
        {15, 0, 37},
        {52, 37, 2},
        {0, 39, 15},
    }};
    almanac["fertilizer-to-water"] = day5::RangeMap<size_t>{{
        {53, 49, 8},
        {11, 0, 42},
        {0, 42, 7},
        {7, 57, 4},
    }};
    almanac["water-to-light"] = day5::RangeMap<size_t>{{
        {18, 88, 7},
        {25, 18, 70},
    }};
    almanac["light-to-temperature"] = day5::RangeMap<size_t>{{
        {77, 45, 23},
        {45, 81, 19},
        {64, 68, 13},
    }};
    almanac["temperature-to-humidity"] = day5::RangeMap<size_t>{{
        {69, 0, 1},
        {0, 1, 69},
    }};
    almanac["humidity-to-location"] = day5::RangeMap<size_t>{{
        {56, 60, 37},
        {93, 56, 4},
    }};

    return almanac;
}

TEST(Day5, Example) {
    auto almanac = example();

    // seed 79
    EXPECT_EQ(almanac["seed-to-soil"].at(79), 81);
    EXPECT_EQ(almanac["soil-to-fertilizer"].at(81), 81);
    EXPECT_EQ(almanac["fertilizer-to-water"].at(81), 81);
    EXPECT_EQ(almanac["water-to-light"].at(81), 74);
    EXPECT_EQ(almanac["light-to-temperature"].at(74), 78);
    EXPECT_EQ(almanac["temperature-to-humidity"].at(78), 78);
    EXPECT_EQ(almanac["humidity-to-location"].at(78), 82);

    // seed 14
    EXPECT_EQ(almanac["seed-to-soil"].at(14), 14);
    EXPECT_EQ(almanac["soil-to-fertilizer"].at(14), 53);
    EXPECT_EQ(almanac["fertilizer-to-water"].at(53), 49);
    EXPECT_EQ(almanac["water-to-light"].at(49), 42);
    EXPECT_EQ(almanac["light-to-temperature"].at(42), 42);
    EXPECT_EQ(almanac["temperature-to-humidity"].at(42), 43);
    EXPECT_EQ(almanac["humidity-to-location"].at(43), 43);

    // seed 55
    EXPECT_EQ(almanac["seed-to-soil"].at(55), 57);
    EXPECT_EQ(almanac["soil-to-fertilizer"].at(57), 57);
    EXPECT_EQ(almanac["fertilizer-to-water"].at(57), 53);
    EXPECT_EQ(almanac["water-to-light"].at(53), 46);
    EXPECT_EQ(almanac["light-to-temperature"].at(46), 82);
    EXPECT_EQ(almanac["temperature-to-humidity"].at(82), 82);
    EXPECT_EQ(almanac["humidity-to-location"].at(82), 86);

    // seed 13
    EXPECT_EQ(almanac["seed-to-soil"].at(13), 13);
    EXPECT_EQ(almanac["soil-to-fertilizer"].at(13), 52);
    EXPECT_EQ(almanac["fertilizer-to-water"].at(52), 41);
    EXPECT_EQ(almanac["water-to-light"].at(41), 34);
    EXPECT_EQ(almanac["light-to-temperature"].at(34), 34);
    EXPECT_EQ(almanac["temperature-to-humidity"].at(34), 35);
    EXPECT_EQ(almanac["humidity-to-location"].at(35), 35);
}

TEST(Day5, ExampleMapLocation) {
    auto almanac = example();

    EXPECT_EQ(almanac.map_location(79).back(), 82);
    EXPECT_EQ(almanac.map_location(14).back(), 43);
    EXPECT_EQ(almanac.map_location(55).back(), 86);
    EXPECT_EQ(almanac.map_location(13).back(), 35);
}

TEST(Day5, ExampleFindSeed) {
    auto almanac = example();

    EXPECT_EQ(almanac.find_seed(82).value(), 79);
    EXPECT_EQ(almanac.find_seed(43).value(), 14);
    EXPECT_EQ(almanac.find_seed(86).value(), 55);
    EXPECT_EQ(almanac.find_seed(35).value(), 13);
    EXPECT_FALSE(almanac.find_seed(0).has_value());
}
