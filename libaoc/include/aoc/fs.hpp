#pragma once

#include <filesystem>
#include <fstream>

namespace aoc {

std::fstream open(const std::filesystem::path& path);

}  // namespace aoc
