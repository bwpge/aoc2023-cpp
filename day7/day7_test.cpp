#include "day7.hpp"

#include <gtest/gtest.h>

using day7::Card;
using day7::Hand;

TEST(Day7, Card) {
    const std::vector<std::pair<char, int>> test_data{
        {'A', 14},
        {'K', 13},
        {'Q', 12},
        {'J', 11},
        {'T', 10},
        {'9', 9},
        {'8', 8},
        {'7', 7},
        {'6', 6},
        {'5', 5},
        {'4', 4},
        {'3', 3},
        {'2', 2},
    };

    for (const auto [c, expected] : test_data) {
        auto card = Card::parse(c);
        EXPECT_EQ(card.value, expected);
    }
}

TEST(Day7, HandKind) {
    std::vector<std::pair<std::string, Hand::Kind>> test_data{
        {"AAAAA", Hand::Kind::FiveKind},
        {"AA8AA", Hand::Kind::FourKind},
        {"23332", Hand::Kind::FullHouse},
        {"TTT98", Hand::Kind::ThreeKind},
        {"23432", Hand::Kind::TwoPair},
        {"A23A4", Hand::Kind::OnePair},
        {"23456", Hand::Kind::HighCard},
    };

    for (const auto& [cards, kind] : test_data) {
        Hand hand{cards};
        EXPECT_EQ(hand.kind(), kind) << "With line = `" << cards << '`';
    }
}

TEST(Day7, HandCompare) {
    Hand high_card{"23456"};
    Hand two_kind1{"KK677"};
    Hand two_kind2{"KTJJT"};
    Hand three_kind{"QQQJA"};
    Hand full_house{"23332"};
    Hand four_kind{"AA8AA"};
    Hand aces{"AAAAA"};

    // same kind, but two_kind2 has a lower second card than two_kind1
    EXPECT_EQ(two_kind1.kind(), two_kind2.kind());
    ASSERT_LT(two_kind2, two_kind1);

    // aces should beat all kinds
    ASSERT_LT(high_card, aces);
    ASSERT_LT(two_kind1, aces);
    ASSERT_LT(two_kind2, aces);
    ASSERT_LT(three_kind, aces);
    ASSERT_LT(full_house, aces);
    ASSERT_LT(four_kind, aces);

    // high card should lose to all other kinds
    ASSERT_LT(high_card, two_kind1);
    ASSERT_LT(high_card, two_kind2);
    ASSERT_LT(high_card, three_kind);
    ASSERT_LT(high_card, full_house);
    ASSERT_LT(high_card, four_kind);
    ASSERT_LT(high_card, aces);
}

TEST(Day7, ExampleParse) {
    std::vector<std::tuple<std::string, Hand::Kind, int>> test_data{
        {"32T3K 765", Hand::Kind::OnePair, 765},
        {"T55J5 684", Hand::Kind::ThreeKind, 684},
        {"KK677 28", Hand::Kind::TwoPair, 28},
        {"KTJJT 220", Hand::Kind::TwoPair, 220},
        {"QQQJA 483", Hand::Kind::ThreeKind, 483},
    };

    for (const auto& [line, kind, bid] : test_data) {
        auto hand = Hand::parse(line);
        EXPECT_EQ(hand.kind(), kind) << "With line = `" << line << '`';
        EXPECT_EQ(hand.bid(), bid) << "With line = `" << line << '`';
    }
}
