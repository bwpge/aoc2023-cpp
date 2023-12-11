#pragma once

#include <source_location>
#include <utility>

#include <fmt/core.h>  // IWYU pragma: export
#include <fmt/format.h>  // IWYU pragma: export
#include <spdlog/spdlog.h>  // IWYU pragma: export

namespace aoc {

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#  define AOC_DEBUGBREAK() __debugbreak()
#else
#  include <signal.h>
#  if defined(SIGTRAP)
#    define AOC_DEBUGBREAK() raise(SIGTRAP)
#  else
#    define AOC_DEBUGBREAK() raise(SIGABRT)
#  endif
#endif

#define XSTRINGIFY(arg) #arg
#define STRINGIFY(arg) XSTRINGIFY(arg)

#define AOC_ASSERT(expr, msg)                                         \
    do {                                                              \
        /* NOLINTNEXTLINE(readability-simplify-boolean-expr) */       \
        if (!(expr)) {                                                \
            spdlog::error("FAILED ASSERTION: `{}`", STRINGIFY(expr)); \
            ::aoc::panic(msg);                                        \
        }                                                             \
    } while (0)

// NOLINTEND(cppcoreguidelines-macro-usage)

template<typename T>
[[noreturn]]
constexpr void panic(T&& msg, std::source_location location = std::source_location::current()) {
    spdlog::
        critical("PANIC: {} ({}:{})", std::forward<T>(msg), location.file_name(), location.line());
    AOC_DEBUGBREAK();
    abort();
}

[[noreturn]]
constexpr void todo(std::source_location location = std::source_location::current()) {
    panic("not implemented", location);
}

[[noreturn]]
constexpr void unreachable(std::source_location location = std::source_location::current()) {
    panic("location should be unreachable", location);
}

template<typename T>
constexpr void unused(T&& arg) {
    static_cast<void>(std::forward<T>(arg));
}

template<typename T, typename... Args>
constexpr void unused(T&& arg, Args... args) {
    unused(std::forward<T>(arg));
    unused(std::forward<Args...>(args...));
}

}  // namespace aoc
