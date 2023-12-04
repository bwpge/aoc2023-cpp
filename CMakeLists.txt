cmake_minimum_required(VERSION 3.27)
project("advent-of-code" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/.clang-tidy" "Checks: '-*'")

set(executables "")
set(AOC_HEADER_FILES
    "include/aoc/digits.hpp"
)

add_executable(
    aoc_test
    "tests/decode_test.cpp"
    "tests/stringutils_test.cpp"
)
list(APPEND executables aoc_test)
add_executable(day1 "src/day1.cpp")
list(APPEND executables day1)
add_executable(day2 "src/day2.cpp")
list(APPEND executables day2)
add_executable(day3 "src/day3.cpp")
list(APPEND executables day3)

# copy input data files
add_custom_target(copy-data
    COMMAND
    ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
)

foreach(exe IN LISTS executables)
    # includes
    target_include_directories(${exe} PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    )

    # compiler specific options
    if(MSVC)
        # use static runtime linking on msvc
        set_property(TARGET ${exe}
            PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        )
        target_compile_options(${exe} PRIVATE /W4 /WX)
    else()
        target_compile_options(${exe} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_dependencies(${exe} copy-data)
endforeach()

# testing
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
find_package("GTest" CONFIG REQUIRED)
target_link_libraries(
  aoc_test
  GTest::gtest
  GTest::gtest_main
)
# add tests to cmake
include(GoogleTest)
gtest_add_tests(TARGET aoc_test)
