#pragma once

#include "aoc/aoc.hpp"

namespace day6 {

class Race {
public:
    using num_t = double;

    Race() = default;

    Race(double time, double distance) : _time{time}, _distance{distance} {}

    [[nodiscard]]
    bool can_win(double charge_ms) const {
        if (charge_ms >= _time) {
            return false;
        }

        auto v{charge_ms};
        auto t{_time - charge_ms};

        return (v * t) > _distance;
    }

    /*
     * This is a quadratic equation, and the solution is fairly straightforward
     * with some quick arithmetic.
     *
     *   D, T are given by the race
     *   v = charge_ms
     *   t = (T - charge_ms)
     *
     * With the solution as:
     *   v * t > D
     * =>
     *   charge_ms(T - charge_ms) > D
     *   -(charge_ms^2) + T*charge_ms - D > 0
     *
     * We can use the quadratic formula, with a=-1, b=T, c=-D
     * (we can also simplify a little bit to avoid introducing more
     * precision issues)
     *
     *        -T +/- sqrt( T^2 - 4(-1)(-D) )
     *    x = -------------------------------
     *                    2(-1)
     *
     * =>
     *        T +/- sqrt( T^2 - 4*D )
     *    x = ------------------------
     *                  2
     *
     * with an assertion that (T^2  >= 4*D) so that the result is a real number
     */
    [[nodiscard]]
    std::pair<int64_t, int64_t> win_condition() const {
        AOC_ASSERT((_time * _time) >= (4.0 * _distance), "Race must have a real number solution");

        auto s = std::sqrt((_time * _time) - (4 * _distance));
        double t_min{(_time - s) / 2.0};
        double t_max{(_time + s) / 2.0};
        t_min = std::round(t_min);
        t_max = std::round(t_max);

        // fix rounding logic issues
        if (!can_win(t_min)) {
            t_min++;
        }
        if (!can_win(t_max)) {
            t_max--;
        }

        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        AOC_ASSERT(can_win(t_min) && can_win(t_max), "Race must have a valid win condition");
        return {static_cast<int64_t>(t_min), static_cast<int64_t>(t_max)};
    }

    [[nodiscard]]
    int64_t margin() const {
        const auto [t_min, t_max] = win_condition();
        return t_max + 1 - t_min;
    }

private:
    double _time{};
    double _distance{};
};

inline int64_t margin_of_error(const std::vector<Race>& races) {
    int64_t ways{1};
    for (const auto& race : races) {
        ways *= race.margin();
    }

    return ways;
}

}  // namespace day6
