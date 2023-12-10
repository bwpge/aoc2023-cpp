#pragma once

#include "aoc/aoc.hpp"

#include <deque>

namespace day9 {

class Reading {
public:
    enum class Direction {
        Future,
        Past,
    };

    static Reading parse(std::string_view s) {
        Reading result{};
        auto values = aoc::split(s, ' ', aoc::SplitOptions::DiscardEmpty);
        std::ranges::transform(values, std::back_inserter(result._nums), [](auto sv) {
            return std::stoll(std::string{sv});
        });

        return result;
    }

    [[nodiscard]]
    int64_t analyze(Direction dir = Direction::Future) const {
        bool future = dir == Direction::Future;
        std::deque<std::deque<int64_t>> stack{{_nums}};
        std::deque<int64_t> current{};

        // generate next sequence until all zeroes
        while (!std::ranges::all_of(stack.front(), [](auto i) { return i == 0; })) {
            const auto& front = stack.front();
            AOC_ASSERT(!front.empty(), "row does not contain any numbers");

            for (size_t i = 0; i < front.size() - 1; ++i) {
                current.push_back(front[i + 1] - front[i]);
            }
            stack.emplace_front(std::move(current));
            current = {};
        }

        // unwind rows to get result
        while (stack.size() > 1) {
            current = stack.front();
            stack.pop_front();
            auto& front = stack.front();

            if (future) {
                auto i = front.back();
                auto j = current.back();
                front.push_back(i + j);
            } else {
                auto i = front.front();
                auto j = current.front();
                front.push_front(i - j);
            }
        }
        current = stack.front();

        return future ? current.back() : current.front();
    }

    [[nodiscard]]
    const std::deque<int64_t>& values() const {
        return _nums;
    }

private:
    std::deque<int64_t> _nums{};
};

}  // namespace day9
