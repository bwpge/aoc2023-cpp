# VCPKG NOTE:
#   configure VCPKG_TARGET_TRIPLET somewhere (either through extension setting
#   or `SET(...)`) prior to the project directive to ensure correct libraries
#   are linked by triplet (e.g., x64-<platform>[-static])

cmake_minimum_required(VERSION 3.27)
project("aoc2023" VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/.clang-tidy" "Checks: '-*'")

# =======
# OPTIONS
# =======

option(AOC_BUILD_TESTS "Build project tests" OFF)

add_subdirectory(libaoc)
if (AOC_BUILD_TESTS)
    message(STATUS "Generating tests")
    enable_testing()
    add_subdirectory(tests)
endif()

# =========
# SOLUTIONS
# =========

set(executables "")

add_executable(day1 "src/day1.cpp")
list(APPEND executables day1)
add_executable(day2 "src/day2.cpp")
list(APPEND executables day2)
add_executable(day3 "src/day3.cpp")
list(APPEND executables day3)
add_executable(day4 "src/day4.cpp")
list(APPEND executables day4)

# ==========
# INPUT DATA
# ==========

add_custom_target(copy-data
    COMMAND
    ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/data" "${CMAKE_BINARY_DIR}/data"
)

# ==========
# EXE CONFIG
# ==========

foreach(exe IN LISTS executables)
    if(MSVC)
        target_compile_options(${exe} PRIVATE /W4 /WX)
    else()
        target_compile_options(${exe} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()

    add_dependencies(${exe} copy-data)
    target_link_libraries(${exe} libaoc::libaoc)
endforeach()
