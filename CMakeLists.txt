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

# ==============
# SUBDIRECTORIES
# ==============

add_subdirectory(libaoc)
if (AOC_BUILD_TESTS)
    message(STATUS "Generating library tests")
    enable_testing()
    add_subdirectory(tests)
endif()

# use this list to enumerate solution directories
# see EXE CONFIG section for usage
set(solutions
    "day1"
    "day2"
    "day3"
    "day4"
    "day5"
    "day6"
    "day7"
)

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

foreach(exe IN LISTS solutions)
    add_executable(${exe} "${exe}/main.cpp")
    target_include_directories(${exe} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${exe}/include>"
    )

    if(MSVC)
        target_compile_options(${exe} PRIVATE /W4 /WX)
    else()
        target_compile_options(${exe} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()

    add_dependencies(${exe} copy-data)
    target_link_libraries(${exe} libaoc::libaoc)
endforeach()

# add tests if enabled
if(AOC_BUILD_TESTS)
    foreach(exe IN LISTS solutions)
        set(test_name "${exe}_test")
        set(test_src "${CMAKE_CURRENT_SOURCE_DIR}/${exe}/${test_name}.cpp")

        if (EXISTS "${test_src}")
            message(STATUS "Generating ${exe} test")
            add_executable(${test_name} "${test_src}")
            target_include_directories(${test_name} PUBLIC
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${exe}/include>"
            )

            set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
            find_package("GTest" CONFIG REQUIRED)
            target_link_libraries(
                ${test_name}
                libaoc::libaoc
                GTest::gtest
                GTest::gtest_main
            )
            gtest_add_tests(TARGET ${test_name})
        endif()
    endforeach()
endif()
