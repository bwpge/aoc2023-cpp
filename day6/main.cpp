#include "day6.hpp"

#include <iostream>

struct RaceData {
    std::string time{};
    std::string distance{};
};

RaceData extract_data(const std::filesystem::path& path) {
    auto input = aoc::open(path);

    RaceData result{};
    std::string line{};
    while (std::getline(input, line)) {
        AOC_ASSERT(line.find(':') != std::string::npos, "Line does not contain required `:`");
        auto [prefix, body] = aoc::split_once(line, ':').value();
        auto numstrs =
            aoc::split(body, ' ', aoc::SplitOptions::DiscardEmpty | aoc::SplitOptions::Trim);

        if (prefix == "Time") {
            result.time = body;
        } else if (prefix == "Distance") {
            result.distance = body;
        } else {
            aoc::panic(fmt::format("Unknown prefix `{}`", prefix));
        }
    }
    return result;
}

void part1(const std::filesystem::path& path) {
    using num_t = day6::Race::num_t;
    auto data = extract_data(path);

    auto split_to_nums = [](const std::string& s) {
        auto strs = aoc::split(s, ' ', aoc::SplitOptions::DiscardEmpty | aoc::SplitOptions::Trim);
        std::vector<num_t> nums{};
        std::ranges::transform(strs, std::back_inserter(nums), [](auto s) {
            return std::stod(std::string{s});
        });
        return nums;
    };

    auto times = split_to_nums(data.time);
    auto distances = split_to_nums(data.distance);
    AOC_ASSERT(times.size() == distances.size(), "Must have equal times and distances");

    std::vector<day6::Race> races{};
    for (auto&& [time, distance] : aoc::zip_copy(times, distances)) {
        races.emplace_back(time, distance);
    }

    auto result = day6::margin_of_error(races);
    std::cout << "Part 1: " << result << '\n';
}

void part2(const std::filesystem::path& path) {
    auto data = extract_data(path);

    auto to_num = [](const std::string& s) {
        auto copy = aoc::remove(s, ' ');
        return stod(copy);
    };

    auto time = to_num(data.time);
    auto distance = to_num(data.distance);
    day6::Race race{time, distance};

    std::cout << "Part 2: " << race.margin() << '\n';
}

int main() {
    std::filesystem::path path{"data/day6.txt"};
    part1(path);
    part2(path);
}
