#pragma once

#include "aoc/core.hpp"
#include "aoc/hash.hpp"
#include "aoc/types.hpp"

namespace aoc {

struct Coord {
    usize x{};
    usize y{};

    auto operator<=>(const Coord& other) const = default;

    [[nodiscard]]
    bool is_adjacent(const Coord& other) const {
        auto dx = static_cast<i64>(x) - static_cast<i64>(other.x);
        auto dy = static_cast<i64>(y) - static_cast<i64>(other.y);

        return std::abs(dx) + std::abs(dy) == 1;
    }
};

}  // namespace aoc

namespace std {
template<>
struct hash<aoc::Coord> {
    size_t operator()(const aoc::Coord& coord) const {
        size_t seed{};
        aoc::hash_combine(seed, coord.x, coord.y);
        return seed;
    }
};
}  // namespace std

namespace aoc {

template<typename T>
class Grid {
public:
    template<std::ranges::range Container, typename Ty>
    friend Grid<Ty> make_grid(const Container&);

    Grid() = default;
    Grid(const Grid&) = default;
    Grid(Grid&&) = default;
    Grid& operator=(const Grid&) = default;
    Grid& operator=(Grid&&) = default;
    virtual ~Grid() = default;

    enum class Direction {
        North,
        East,
        South,
        West
    };

    static Direction direction(Coord from, Coord to) {
        auto dx = static_cast<i64>(to.x) - static_cast<i64>(from.x);
        auto dy = static_cast<i64>(to.y) - static_cast<i64>(from.y);

        AOC_ASSERT(
            (std::abs(dx) + std::abs(dy)) == 1,
            "coordinates must be adjacent in a cardinal direction"
        );

        if (dy > 0) {
            return Direction::South;
        }
        if (dy < 0) {
            return Direction::North;
        }
        if (dx > 0) {
            return Direction::East;
        }
        if (dx < 0) {
            return Direction::West;
        }

        aoc::unreachable();
    }

    [[nodiscard]]
    std::vector<Coord> adjacent(Coord pos) const {
        AOC_ASSERT(contains(pos), "grid must contain position");
        std::vector<Coord> result{};

        // north
        if (pos.y > 0) {
            result.emplace_back(pos.x, pos.y - 1);
        }
        // east
        if (pos.x + 1 < width()) {
            result.emplace_back(pos.x + 1, pos.y);
        }
        // south
        if (pos.y + 1 < height()) {
            result.emplace_back(pos.x, pos.y + 1);
        }
        // west
        if (pos.x > 0) {
            result.emplace_back(pos.x - 1, pos.y);
        }

        return result;
    }

    [[nodiscard]]
    std::vector<Coord> adjacent(usize x, usize y) const {
        return adjacent({x, y});
    }

    [[nodiscard]]
    virtual size_t min_distance(const Coord& from, const Coord& to) const {
        int64_t dx = static_cast<int64_t>(from.x) - static_cast<int64_t>(to.x);
        int64_t dy = static_cast<int64_t>(from.y) - static_cast<int64_t>(to.y);
        return std::abs(dx) + std::abs(dy);
    };

    [[nodiscard]]
    constexpr const T& at(Coord pos) const {
        return _inner.at(pos.y).at(pos.x);
    }

    [[nodiscard]]
    constexpr const T& at(usize x, usize y) const {
        return _inner.at(y).at(x);
    }

    [[nodiscard]]
    constexpr T& at(Coord pos) {
        return _inner.at(pos.y).at(pos.x);
    }

    [[nodiscard]]
    constexpr T& at(usize x, usize y) {
        return _inner.at(y).at(x);
    }

    [[nodiscard]]
    constexpr usize width() const {
        return _inner.front().size();
    }

    [[nodiscard]]
    constexpr usize height() const {
        return _inner.size();
    }

    [[nodiscard]]
    constexpr bool contains(const Coord& coord) const {
        return coord.x < width() && coord.y < height();
    }

    void insert_row(usize offset, const T& fill) {
        AOC_ASSERT(offset <= width(), "row offset out of range");
        _inner.insert(_inner.begin() + offset, std::vector<T>(width(), fill));
    }

    void insert_col(usize offset, const T& fill) {
        AOC_ASSERT(offset <= height(), "column offset out of range");
        for (auto& col : _inner) {
            col.insert(col.begin() + offset, fill);
        }
    }

private:
    std::vector<std::vector<T>> _inner{};
};

template<std::ranges::range Container, typename T = range_val<range_val<Container>>>
static Grid<T> make_grid(const Container& data) {
    AOC_ASSERT(!std::ranges::empty(data), "grid must have at least one row");

    Grid<T> result{};
    for (const auto& row : data) {
        AOC_ASSERT(!std::ranges::empty(row), "grid must have at least one column");

        std::vector<T> items{};
        for (const auto& el : row) {
            static_assert(std::is_same_v<std::remove_cvref_t<decltype(el)>, T>);
            items.push_back(el);
        }
        result._inner.push_back(std::move(items));
    }

    // verify dimensions
    usize width{result._inner.front().size()};
    for (const auto& row : result._inner) {
        AOC_ASSERT(row.size() == width, "all grid rows must be of equal size");
    }

    return result;
}

}  // namespace aoc
