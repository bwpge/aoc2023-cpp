#pragma once

#include <source_location>
#include <utility>

#include <fmt/core.h>  // IWYU pragma: export
#include <fmt/format.h>  // IWYU pragma: export
#include <spdlog/spdlog.h>  // IWYU pragma: export

namespace aoc {

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define XSTRINGIFY(arg) #arg
#define STRINGIFY(arg) XSTRINGIFY(arg)

#define AOC_ASSERT(expr, msg)                                            \
    do {                                                                 \
        if (!(expr)) {                                                   \
            spdlog::critical("FAILED ASSERTION: `{}`", STRINGIFY(expr)); \
            ::aoc::panic(msg);                                           \
        }                                                                \
    } while (0)

// NOLINTEND(cppcoreguidelines-macro-usage)

template<typename T>
[[noreturn]]
constexpr void panic(T&& msg, std::source_location location = std::source_location::current()) {
    spdlog::
        critical("PANIC: {} ({}:{})", std::forward<T>(msg), location.file_name(), location.line());
    abort();
}

}  // namespace aoc
