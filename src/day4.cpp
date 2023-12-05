#include "aoc/aoc.hpp"

#include <iostream>
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
    size_t points() const {
        int matches{};
        for (const auto& num : _numbers) {
            if (_winning.contains(num)) {
                matches++;
            }
        }

        if (matches <= 0) {
            return 0;
        }

        return 1ULL << (matches - 1);
    }

private:
    int _id{};
    std::unordered_set<std::string> _winning{};
    std::vector<std::string> _numbers{};
};

void part1() {
    auto input = aoc::open("data/day4.txt");

    size_t sum{};
    std::string line{};
    while (std::getline(input, line)) {
        auto card = Card::parse(line);
        sum += card.points();
    }

    std::cout << "Result: " << sum << '\n';
}

int main() noexcept(false) {
    part1();
}
