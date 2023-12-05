#include "aoc/aoc.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

int day1(const std::filesystem::path& path) {
    std::fstream input{path};
    if (!input) {
        throw std::runtime_error{"Failed to read file"};
    }

    int count = 0;
    std::string line{};
    std::vector<int> nums{};
    while (std::getline(input, line)) {
        count++;
        std::optional<int> x{};
        std::optional<int> y{};

        // find first digit
        for (size_t i = 0; i < line.length(); i++) {
            x = aoc::decode_digit(line, i);
            if (x.has_value()) {
                break;
            }
        }

        // find last digit
        for (size_t i = line.length() - 1; i >= 0; i--) {
            y = aoc::decode_digit(line, i);
            if (y.has_value()) {
                break;
            }
        }

        auto num = x.value() * 10 + y.value();
        std::cout << "Line " << count << ": " << num << '\n';
        nums.push_back(num);
    }

    return std::reduce(nums.begin(), nums.end());
}

int main() noexcept(false) {
    auto result = day1("data/day1.txt");
    std::cout << "Result: " << result << "\n";

    return 0;
}
