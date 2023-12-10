#pragma once

#include "aoc/aoc.hpp"

#include <deque>

namespace day9 {

class Reading {
    using num_type = int64_t;

public:
    static Reading parse(std::string_view s) {
        Reading result{};
        auto values = aoc::split(s, ' ', aoc::SplitOptions::DiscardEmpty);
        std::ranges::transform(values, std::back_inserter(result._nums), [](auto sv) {
            return std::stoll(std::string{sv});
        });

        return result;
    }

    [[nodiscard]]
    num_type predict() const {
        std::deque<std::vector<num_type>> stack{{_nums}};
        std::vector<num_type> current{};

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

        // unwind rows to get prediction
        while (stack.size() > 1) {
            current = stack.front();
            stack.pop_front();
            auto& front = stack.front();

            auto i = front.back();
            auto j = current.back();
            front.push_back(i + j);
        }

        return stack.front().back();
    }

    [[nodiscard]]
    num_type past() const {
        std::deque<std::deque<num_type>> stack{std::deque<num_type>{_nums.begin(), _nums.end()}};
        std::deque<num_type> current{};

        // generate next sequence until all zeroes
        while (!std::ranges::all_of(stack.front(), [](auto i) { return i == 0; })) {
            const auto& front = stack.front();
            AOC_ASSERT(!front.empty(), "row does not contain any numbers");

            for (size_t i = front.size() - 1; i > 0; --i) {
                current.push_front(front[i] - front[i - 1]);
            }
            stack.emplace_front(std::move(current));
            current = {};
        }

        // unwind rows to get prediction
        while (stack.size() > 1) {
            current = stack.front();
            stack.pop_front();
            auto& front = stack.front();

            auto i = front.front();
            auto j = current.front();
            front.push_front(i - j);
        }

        return stack.front().front();
    }

    [[nodiscard]]
    const std::vector<int64_t>& values() const {
        return _nums;
    }

private:
    std::vector<int64_t> _nums{};
};

}  // namespace day9
