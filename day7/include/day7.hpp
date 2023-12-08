#pragma once

#include "aoc/aoc.hpp"

#include <ostream>
#include <unordered_map>
#include <utility>

namespace day7 {

const std::string LABELS{"23456789TJQKA"};

struct RuleSet {
    std::string labels{"23456789TJQKA"};
    std::vector<char> wild{};

    [[nodiscard]]
    bool is_wild(char c) const {
        return std::ranges::find(wild, c) != wild.end();
    }

    [[nodiscard]]
    bool is_wild(int i) const {
        if (i >= labels.length() || i < 0) {
            return false;
        }
        return is_wild(labels.at(i));
    }
};

const RuleSet JOKERS_WILD{"J23456789TQKA", {'J'}};

struct Card {
    int value{};

    static Card parse(char c, const RuleSet& rules) {
        auto idx = rules.labels.find(c);
        if (idx == std::string::npos) {
            aoc::panic(fmt::format("unknown card `{}`", c));
        }

        return Card{static_cast<int>(idx)};
    }

    static std::vector<Card> parse(std::string_view s, const RuleSet& rules) {
        std::vector<Card> cards{};
        std::transform(s.begin(), s.end(), std::back_inserter(cards), [&rules](char c) {
            return Card::parse(c, rules);
        });

        return cards;
    }

    bool operator==(const Card& other) const {
        return value == other.value;
    }

    bool operator<(const Card& other) const {
        return value < other.value;
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

    explicit Hand(std::string_view s, RuleSet rules = {}) : _rules{std::move(rules)} {
        _cards = Card::parse(s, _rules);
    }

    static Hand parse(std::string_view s, RuleSet rules = {}) {
        auto [cards, bid] = aoc::split_once(s, ' ').value();
        Hand hand{};

        hand._cards = Card::parse(cards, rules);
        hand._bid = std::stoull(std::string{bid});
        hand._rules = std::move(rules);

        return hand;
    }

    [[nodiscard]]
    bool has_wilds() const {
        return !_rules.wild.empty() || std::ranges::any_of(_cards, [this](const Card& c) {
            return _rules.is_wild(c.value);
        });
    }

    [[nodiscard]]
    Kind kind() const {
        if (has_wilds()) {
            return kind_wild_impl();
        }
        return Hand::kind_impl(_cards);
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

    // additional comparison logic from: https://stackoverflow.com/a/18289698

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
        std::transform(_cards.begin(), _cards.end(), std::back_inserter(result), [this](auto c) {
            return card_to_char(c);
        });

        return result;
    }

private:
    [[nodiscard]]
    static Kind kind_impl(const std::vector<Card>& cards) {
        std::unordered_map<int, int> counts{};
        for (const auto& card : cards) {
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

    static std::vector<std::vector<Card>> wild_combinations(
        const std::vector<Card>& cards,
        const std::vector<Card>& choices,
        size_t count
    ) {
        // combination logic from: https://stackoverflow.com/a/39680841
        //   base = choices.size()
        //   length = cards.size() + count
        //   total combinations = base^length

        if (count <= 0) {
            return {cards};
        }

        size_t base = choices.size();
        size_t length = cards.size() + count;
        size_t total = 1;
        for (size_t i = 0; i < count; ++i) {
            total *= base;
        }

        std::vector<std::vector<Card>> result{};
        for (size_t i = 0; i < total; i++) {
            std::vector<Card> c{cards.begin(), cards.end()};

            for (auto n = i; n > 0;) {
                auto d = n % base;
                c.push_back(choices.at(d));
                n /= base;
            }
            while (c.size() < length) {
                c.push_back(choices.at(0));
            }
            result.push_back(c);
        }

        return result;
    }

    [[nodiscard]]
    Kind kind_wild_impl() const {
        // the idea here is to build out each possible hand, subbing for each wild card.
        // we can limit the choices to what non-wildcards are in the hand, because this
        // game does not have any "straight" or "flush" types where a sequence is worth
        // more than a high card hand -- that means the only value a wild card adds is
        // when it matches a card currently in the hand. the game is not clear on whether
        // 5 wilds can be considered five of a kind, but seems reasonable to assume.

        if (std::ranges::all_of(_cards, [this](const Card& c) {
                return _rules.is_wild(c.value);
            })) {
            return Kind::FiveKind;
        }

        size_t wilds{};
        std::vector<Card> cards{};
        std::vector<Card> choices{};
        for (const auto& c : _cards) {
            if (!_rules.is_wild(c.value)) {
                cards.push_back(c);
                if (std::ranges::find(choices, c) == choices.end()) {
                    choices.push_back(c);
                }
            } else {
                wilds++;
            }
        }

        if (wilds == 0) {
            return Hand::kind_impl(cards);
        }

        Kind result{};
        auto combinations = Hand::wild_combinations(cards, choices, wilds);
        for (const auto& hand : combinations) {
            auto k = std::max(result, Hand::kind_impl(hand));
            // return early if we get the best kind
            if (k == Kind::FiveKind) {
                return k;
            }
            result = std::max(result, k);
        }

        return result;
    }

    [[nodiscard]]
    char card_to_char(const Card& card) const {
        return _rules.labels.at(card.value);
    }

    std::vector<Card> _cards{};
    size_t _bid{};
    RuleSet _rules{};
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
