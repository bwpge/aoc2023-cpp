#include "aoc/stringutils.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

struct GameSet {
    int red{};
    int green{};
    int blue{};
};

class Game {
public:
    static Game parse(const std::string& s) {
        Game game{};

        auto [prefix, body] = aoc::split_once(s, ": ").value();
        auto [lit, numstr] = aoc::split_once(prefix, ' ').value();
        if (lit != "Game") {
            std::cout << "ERROR: Unexpected token `" << lit << "`\n";
        }
        game._id = std::stoi(std::string{numstr});

        auto samples = aoc::split(body, "; ");
        for (const auto sample : samples) {
            auto cubes = aoc::split(sample, ", ");
            GameSet set{};

            for (auto cube : cubes) {
                auto [countstr, color] = aoc::split_once(cube, ' ').value();
                auto count = std::stoi(std::string{countstr});
                if (color == "red") {
                    set.red = count;
                } else if (color == "green") {
                    set.green = count;
                } else if (color == "blue") {
                    set.blue = count;
                } else {
                    throw std::runtime_error{"Unknown color"};
                }
            }
            game._sets.push_back(set);
        }

        return game;
    }

    [[nodiscard]]
    auto id() const {
        return _id;
    }

    [[nodiscard]]
    bool is_possible(const GameSet& max_set) const {
        return std::ranges::all_of(_sets, [&max_set](const GameSet& s) {
            return max_set.red >= s.red && max_set.green >= s.green && max_set.blue >= s.blue;
        });
    }

    [[nodiscard]]
    uint64_t power() const {
        GameSet min_set{};
        for (const auto& set : _sets) {
            min_set.red = std::max(min_set.red, set.red);
            min_set.green = std::max(min_set.green, set.green);
            min_set.blue = std::max(min_set.blue, set.blue);
        }

        auto r = static_cast<uint64_t>(min_set.red);
        auto g = static_cast<uint64_t>(min_set.green);
        auto b = static_cast<uint64_t>(min_set.blue);
        return r * g * b;
    }

private:
    int _id{};
    std::vector<GameSet> _sets{};
};

std::vector<Game> day2(const std::filesystem::path& path) {
    std::fstream input{path};
    if (!input) {
        throw std::runtime_error{"Failed to read file"};
    }

    std::vector<Game> games{};
    std::string line{};
    while (std::getline(input, line)) {
        games.emplace_back(Game::parse(line));
    }

    return games;
}

void part1(const std::vector<Game>& games) {
    int sum{};
    for (const auto& game : games) {
        if (game.is_possible({12, 13, 14})) {
            sum += game.id();
        }
    }
    std ::cout << "Part 1: " << sum << '\n';
}

void part2(const std::vector<Game>& games) {
    uint64_t sum{};
    for (const auto& game : games) {
        sum += game.power();
    }
    std::cout << "Part 2: " << sum << '\n';
}

int main() noexcept(false) {
    auto games = day2("data/day2.txt");
    part1(games);
    part2(games);
}
