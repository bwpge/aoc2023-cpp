#pragma once

#include <functional>

namespace aoc {

// taken directly from boost hash.hpp
// see: https://www.boost.org/doc/libs/1_55_0/boost/functional/hash/hash.hpp
template<typename T>
void hash_combine(std::size_t& seed, const T& v) {
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename T, typename... Ts>
void hash_combine(std::size_t& seed, const T& v, Ts&&... vs) {
    hash_combine(seed, v);
    hash_combine(seed, std::forward<Ts>(vs)...);
}

}  // namespace aoc
