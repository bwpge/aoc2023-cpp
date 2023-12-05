#include "aoc/aoc.hpp"

#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

class Card {
public:
    static Card parse(const std::string& line) {
        auto sv_to_int = [](std::string_view sv) {
            std::string s{sv};
            return std::stoi(s);
        };

        Card card{};
        const auto [prefix, numbers] = aoc::split_once(line, ": ").value();
        const auto [lit, id] = aoc::split_once(prefix, ' ').value();
        card._id = sv_to_int(aoc::trim(id));

        auto [winningstr, numstr] = aoc::split_once(numbers, " | ").value();
        for (auto number : aoc::split(
                 winningstr,
                 ' ',
                 aoc::SplitOptions::Trim | aoc::SplitOptions::DiscardEmpty
             )) {
            card._winning.insert(std::string{number});
        }

        for (auto number :
             aoc::split(numstr, ' ', aoc::SplitOptions::Trim | aoc::SplitOptions::DiscardEmpty)) {
            card._numbers.emplace_back(number);
        }

        return card;
    }

    [[nodiscard]]
    int id() const {
        return _id;
    }

    [[nodiscard]]
    int matches() const {
        int count{};
        for (const auto& num : _numbers) {
            if (_winning.contains(num)) {
                count++;
            }
        }
        return count;
    }

    [[nodiscard]]
    size_t points() const {
        auto count = static_cast<size_t>(matches());
        if (count <= 0) {
            return 0;
        }

        return 1ULL << (count - 1);
    }

private:
    int _id{};
    std::unordered_set<std::string> _winning{};
    std::vector<std::string> _numbers{};
};

std::vector<Card> parse_cards(const std::filesystem::path& path) {
    auto input = aoc::open(path);

    std::vector<Card> cards{};
    std::string line{};
    while (std::getline(input, line)) {
        cards.emplace_back(Card::parse(line));
    }

    return cards;
}

void part1(const std::vector<Card>& cards) {
    size_t sum{};
    for (const auto& card : cards) {
        sum += card.points();
    }

    std::cout << "Part 1: " << sum << '\n';
}

void part2(const std::vector<Card>& cards) {
    std::map<int, int> counts{};

    auto inc_count = [&counts](int id, int amount = 1) {
        if (!counts.contains(id)) {
            counts[id] = amount;
        } else {
            counts[id] += amount;
        }
    };

    for (const auto& card : cards) {
        // this is the "original" card, so we need to track it in the map.
        // no matter what, the count always increments by 1 since this is
        // the first time we're seeing it
        auto id = card.id();
        inc_count(id);

        auto start = card.id() + 1;
        auto last = start + static_cast<int>(card.matches());
        auto copies = counts[id];

        // generate sequential copies for each copy of the current card
        for (auto i : std::views::iota(start, last)) {
            inc_count(i, copies);
        }
    }

    int sum{};
    for (const auto& [_, count] : counts) {
        sum += count;
    }
    std::cout << "Part 2: " << sum << '\n';
}

int main() noexcept(false) {
    auto cards = parse_cards("data/day4.txt");
    part1(cards);
    part2(cards);
}
