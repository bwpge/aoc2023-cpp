#pragma once

#include <source_location>
#include <utility>

#include <fmt/core.h>  // IWYU pragma: export
#include <fmt/format.h>  // IWYU pragma: export
#include <spdlog/spdlog.h>  // IWYU pragma: export

namespace aoc {

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
  #define AOC_DEBUGBREAK() __debugbreak()
#else
  #include <signal.h>
  #if defined(SIGTRAP)
    #define AOC_DEBUGBREAK() raise(SIGTRAP)
  #else
    #define AOC_DEBUGBREAK() raise(SIGABRT)
  #endif
#endif

#define XSTRINGIFY(arg) #arg
#define STRINGIFY(arg) XSTRINGIFY(arg)

#define AOC_ASSERT(expr, msg)                                         \
    do {                                                              \
        if (!(expr)) {                                                \
            spdlog::error("FAILED ASSERTION: `{}`", STRINGIFY(expr)); \
            ::aoc::panic(msg);                                        \
        }                                                             \
    } while (0)

#define AOC_TODO() ::aoc::panic("Not implemented")

// NOLINTEND(cppcoreguidelines-macro-usage)

template<typename T>
[[noreturn]]
constexpr void panic(T&& msg, std::source_location location = std::source_location::current()) {
    spdlog::
        critical("PANIC: {} ({}:{})", std::forward<T>(msg), location.file_name(), location.line());
    AOC_DEBUGBREAK();
    abort();
}

}  // namespace aoc
