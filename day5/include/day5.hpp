#pragma once

#include "aoc/aoc.hpp"

#include <filesystem>
#include <list>
#include <ranges>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace day5 {

template<std::integral T>
struct Seed {
    T value{};
    T length{};

    [[nodiscard]]
    bool contains(T val) const {
        return val >= value && val < value + length;
    }

    bool operator==(const Seed<T>& rhs) const {
        return value == rhs.value && length == rhs.length;
    }
};
}  // namespace day5

namespace std {
template<>
struct hash<day5::Seed<size_t>> {
    size_t operator()(const day5::Seed<size_t>& seed) const {
        return std::hash<size_t>()(seed.value) ^ (std::hash<size_t>()(seed.length) << 1);
    }
};
}  // namespace std

namespace day5 {

template<std::integral T>
struct Range {

    T src{};
    T dst{};
    T length{};

    [[nodiscard]]
    bool contains(T value) const {
        return value >= src && value < src + length;
    }

    [[nodiscard]]
    bool rcontains(T value) const {
        return value >= dst && value < dst + length;
    }

    [[nodiscard]]
    T offset(T value) const {
        AOC_ASSERT(contains(value), "Range does not contain value");
        auto off = value - src;
        return dst + off;
    }

    [[nodiscard]]
    T roffset(T value) const {
        AOC_ASSERT(rcontains(value), "Range does not contain value");
        auto off = value - dst;
        return src + off;
    }
};

template<std::integral T>
struct RangeMap {
    std::vector<Range<T>> inner{};

    [[nodiscard]]
    bool contains(const T& value) const {
        return std::ranges::any_of(inner, [value](auto r) { return r.contains(value); });
    }

    [[nodiscard]]
    bool rcontains(const T& value) const {
        return std::ranges::any_of(inner, [value](auto r) { return r.rcontains(value); });
    }

    [[nodiscard]]
    T at(const T& value) const {
        for (const auto& range : inner) {
            if (range.contains(value)) {
                return range.offset(value);
            }
        }

        return value;
    }

    [[nodiscard]]
    T rev_at(const T& value) const {
        for (const auto& range : inner) {
            if (range.rcontains(value)) {
                return range.roffset(value);
            }
        }

        return value;
    }

    void add(const Range<T>& other) {
        inner.push_back(other);
    }
};

template<std::integral T>
std::unordered_set<Seed<T>> gen_seed_range(std::vector<T>& nums) {
    std::unordered_set<Seed<T>> result{};
    for (size_t i = 0, j = 1; j < nums.size(); i += 2, j += 2) {
        auto s = Seed{nums.at(i), nums.at(j)};
        spdlog::debug("Generate seed range: start={}, length={}", s.value, s.length);
        result.insert(s);
    }
    return result;
}

class Almanac {
public:
    static Almanac parse(const std::filesystem::path& path, bool use_seed_ranges = false) {
        auto input = aoc::open(path);

        Almanac almanac{};
        std::string curr_map{};

        std::string line{};
        while (std::getline(input, line)) {
            if (line.empty()) {
                continue;
            }

            // parse seeds
            if (line.starts_with("seeds:")) {
                spdlog::info("Parsing seed numbers");
                auto [_, values] = aoc::split_once(line, ": ").value();
                auto seedstr = aoc::
                    split(values, ' ', aoc::SplitOptions::Trim | aoc::SplitOptions::DiscardEmpty);
                std::vector<size_t> nums{};
                std::transform(
                    seedstr.begin(),
                    seedstr.end(),
                    std::back_inserter(nums),
                    [](auto s) { return std::stoull(std::string{s}); }
                );

                if (use_seed_ranges) {
                    almanac._seeds = gen_seed_range(nums);
                } else {
                    std::transform(
                        nums.begin(),
                        nums.end(),
                        std::inserter(almanac._seeds, almanac._seeds.end()),
                        [](auto s) {
                            return Seed{s, 1ULL};
                        }
                    );
                }

                continue;
            }

            // parse map section
            if (line.ends_with(':')) {
                curr_map = aoc::split(line, ' ')[0];
                spdlog::info("Parsed map name: `{}`", curr_map);
                continue;
            }

            // parse triplet
            auto splits = aoc::split(line, ' ');
            std::vector<size_t> nums{};
            std::transform(splits.begin(), splits.end(), std::back_inserter(nums), [](auto s) {
                return std::stoull(std::string{s});
            });
            AOC_ASSERT(nums.size() == 3, "Mapping should have exactly 3 numbers");
            Range<size_t> range{nums[1], nums[0], nums[2]};
            spdlog::
                trace("Adding range src={}, dst={}, count={}", range.src, range.dst, range.length);
            almanac._maps[curr_map].add(range);
        }

        return almanac;
    }

    [[nodiscard]]
    std::list<size_t> map_location(size_t seed) const {
        std::list<size_t> mapping{};
        mapping.push_back(seed);
        for (const auto& name : _keys) {
            const auto& map = _maps.at(name);
            size_t val = mapping.back();
            if (map.contains(val)) {
                val = map.at(val);
            }
            mapping.push_back(val);
        }

        AOC_ASSERT(mapping.size() == 8, "Mapping should have exactly 8 elements");
        return mapping;
    }

    [[nodiscard]]
    std::optional<size_t> find_seed(size_t loc) {
        size_t value = loc;
        for (const auto& name : std::views::reverse(_keys)) {
            const auto& map = _maps.at(name);
            if (map.rcontains(value)) {
                value = map.rev_at(value);
            }
        }

        if (std::ranges::any_of(_seeds, [value](auto r) { return r.contains(value); })) {
            return value;
        }
        return {};
    }

    [[nodiscard]]
    const std::unordered_set<Seed<size_t>>& seeds() const {
        return _seeds;
    }

    void set_seeds(const std::set<size_t>& seeds) {
        _seeds.clear();
        std::
            transform(seeds.begin(), seeds.end(), std::inserter(_seeds, _seeds.end()), [](auto s) {
                return Seed{s, 1ULL};
            });
    }

    RangeMap<size_t>& operator[](const std::string& key) {
        return _maps[key];
    }

private:
    std::unordered_set<Seed<size_t>> _seeds{};
    std::vector<std::string> _keys{
        "seed-to-soil",
        "soil-to-fertilizer",
        "fertilizer-to-water",
        "water-to-light",
        "light-to-temperature",
        "temperature-to-humidity",
        "humidity-to-location",
    };
    std::unordered_map<std::string, RangeMap<size_t>> _maps{
        {"seed-to-soil", {}},
        {"soil-to-fertilizer", {}},
        {"fertilizer-to-water", {}},
        {"water-to-light", {}},
        {"light-to-temperature", {}},
        {"temperature-to-humidity", {}},
        {"humidity-to-location", {}},
    };
};

}  // namespace day5
