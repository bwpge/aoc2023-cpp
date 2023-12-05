#pragma once

#include <filesystem>
#include <fstream>

namespace aoc {

inline std::fstream open(const std::filesystem::path& path) {
    std::fstream input{path};
    if (!input) {
        throw std::runtime_error{"Failed to read file"};
    }

    return input;
}

}  // namespace aoc
