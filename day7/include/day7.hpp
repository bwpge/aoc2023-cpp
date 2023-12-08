#pragma once

#include "aoc/aoc.hpp"

#include <ostream>
#include <unordered_map>

namespace day7 {

const std::string LABELS{"23456789TJQKA"};

struct Card {
    int value{};

    static Card parse(char c) {
        auto value = LABELS.find(c);
        if (value == std::string::npos) {
            aoc::panic(fmt::format("unknown card label `{}`", c));
        }

        return Card{static_cast<int>(value) + 2};
    }

    static std::vector<Card> parse(std::string_view s) {
        std::vector<Card> cards{};
        std::transform(s.begin(), s.end(), std::back_inserter(cards), [](char c) {
            return Card::parse(c);
        });

        return cards;
    }

    bool operator==(const Card& other) const {
        return value == other.value;
    }

    bool operator<(const Card& other) const {
        return value < other.value;
    }

    explicit operator char() const {
        return LABELS.at(value - 2);
    }
};

class Hand {
public:
    enum class Kind {
        HighCard,
        OnePair,
        TwoPair,
        ThreeKind,
        FullHouse,
        FourKind,
        FiveKind,
    };

    Hand() = default;

    explicit Hand(std::string_view s) {
        _cards = Card::parse(s);
    }

    static Hand parse(std::string_view s) {
        auto [cards, bid] = aoc::split_once(s, ' ').value();
        Hand hand{};

        hand._cards = Card::parse(cards);
        hand._bid = std::stoi(std::string{bid});

        return hand;
    }

    [[nodiscard]]
    Kind kind() const {
        std::unordered_map<int, int> counts{};
        for (const auto& card : _cards) {
            if (!counts.contains(card.value)) {
                counts[card.value] = 1;
            } else {
                counts[card.value]++;
            }
        }

        std::vector<int> values{};
        values.reserve(counts.size());
        for (const auto& [_, count] : counts) {
            values.push_back(count);
        }
        std::ranges::sort(values, std::greater<>());

        AOC_ASSERT(!values.empty(), "Hand must have at least one unique value");

        switch (values.size()) {
            case 1:
                return Kind::FiveKind;
            case 2: {
                if (values.at(0) == 4) {
                    return Kind::FourKind;
                }
                return Kind::FullHouse;
            }
            case 3: {
                if (values.at(0) == 3) {
                    return Kind::ThreeKind;
                }
                return Kind::TwoPair;
            }
            case 4:
                return Kind::OnePair;
            default:
                break;
        }

        if (std::ranges::all_of(values, [](auto v) { return v == 1; })) {
            return Kind::HighCard;
        }

        aoc::panic("Unknown hand kind");
    }

    bool operator<(const Hand& other) const {
        AOC_ASSERT(
            _cards.size() == other._cards.size(),
            "Hands must have the same number of cards to compare"
        );

        // hand kind takes priority
        auto k1 = kind();
        auto k2 = other.kind();
        if (k1 != k2) {
            return k1 < k2;
        }

        // otherwise, check the first different card
        for (size_t i = 0; i < _cards.size(); ++i) {
            auto c1 = _cards.at(i);
            auto c2 = other._cards.at(i);
            if (c1 != c2) {
                return c1 < c2;
            }
        }

        return false;
    }

    bool operator==(const Hand& other) const {
        AOC_ASSERT(
            _cards.size() == other._cards.size(),
            "Hands must have the same number of cards to compare"
        );

        return _cards == other._cards;
    }

    // additional comparison logic from: https://stackoverflow.com/a/18289698/

    bool operator<=(const Hand& other) const {
        return !(other < *this);
    }

    bool operator>(const Hand& other) const {
        return other < *this;
    }

    bool operator>=(const Hand& other) const {
        return !(*this < other);
    }

    [[nodiscard]]
    auto cards() const {
        return _cards;
    }

    [[nodiscard]]
    auto bid() const {
        return _bid;
    }

    [[nodiscard]]
    std::string to_string() const {
        std::string result{};
        result.reserve(_cards.size());
        std::transform(_cards.begin(), _cards.end(), std::back_inserter(result), [](auto c) {
            return static_cast<char>(c);
        });

        return result;
    }

private:
    std::vector<Card> _cards{};
    int _bid{};
};

inline std::ostream& operator<<(std::ostream& os, const Hand::Kind& self) {
    switch (self) {
        case Hand::Kind::HighCard:
            return os << "HighCard";
        case Hand::Kind::OnePair:
            return os << "OnePair";
        case Hand::Kind::TwoPair:
            return os << "TwoPair";
        case Hand::Kind::ThreeKind:
            return os << "ThreeKind";
        case Hand::Kind::FullHouse:
            return os << "FullHouse";
        case Hand::Kind::FourKind:
            return os << "FourKind";
        case Hand::Kind::FiveKind:
            return os << "FiveKind";
    }

    return os << "Kind::Unknown";
}

inline std::ostream& operator<<(std::ostream& os, const Hand& hand) {
    return os << hand.to_string();
}

}  // namespace day7
