# aoc2023-cpp

Advent of Code 2023 solutions, written in C++.

## Build

This project uses [CMake](https://cmake.org/) to build and [`vcpkg`](https://vcpkg.io/en/) to manage dependencies. Assuming both are installed, you can run:

```sh
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-YOUR_PLATFORM-static -B . -S ..
cmake --build . --target all
```

Be sure to change `YOUR_PLATFORM` to an appropriate triplet, e.g., `x64-windows-static`.

## Problems

Markdown versions of each problem are stored as README's for each solution directory (e.g., `day1`, `day2`, etc.).

| Day | Title |
|-----|-------|
| 1 | [Trebuchet?!](day1/README.md) |
| 2 | [Cube Conundrum](README/day2.md) |
| 3 | [Gear Ratios](day3/README.md) |
| 4 | [Scratchcards](day4/README.md) |

## Testing

Tests are disabled by default since `GTest` is quite heavy and can take awhile to build. You can enable tests with the `AOC_BUILD_TESTS` option. This may be specified either through the CMake GUI or with the command line option `-DAOC_BUILD_TESTS=ON`.

If using the CMake extension for VSCode, options can also be set in `.vscode/settings.json`:

```jsonc
{
    "cmake.configureArgs": [
        // other args...
        "-DAOC_BUILD_TESTS=ON"
    ]
}
```
