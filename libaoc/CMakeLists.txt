cmake_minimum_required(VERSION 3.27)
project(libaoc VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# ==============
# PROJECT SOURCE
# ==============

set(ENGINE_HEADER_FILES
    "include/aoc/aoc.hpp"
    "include/aoc/digit.hpp"
    "include/aoc/fs.hpp"
    "include/aoc/stringutils.hpp"
)

set(ENGINE_SRC_FILES
    "src/digit.cpp"
    "src/fs.cpp"
    "src/stringutils.cpp"
)

add_library(${PROJECT_NAME} STATIC ${ENGINE_HEADER_FILES} ${ENGINE_SRC_FILES})
add_library(libaoc::libaoc ALIAS libaoc)

target_precompile_headers(${PROJECT_NAME}
    PRIVATE
        "<array>"
        "<algorithm>"
        "<cctype>"
        "<filesystem>"
        "<fstream>"
        "<iostream>"
        "<optional>"
        "<ostream>"
        "<source_location>"
        "<string>"
        "<string_view>"
        "<sstream>"
        "<vector>"
        "<utility>"
)

# ================
# COMPILER OPTIONS
# ================

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# ============
# DEPENDENCIES
# ============

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)

set(packages
    "fmt"
    "spdlog"
)

foreach(pkg ${packages})
    message(STATUS "Adding package: ${pkg}")
    find_package(${pkg} CONFIG REQUIRED)
endforeach()

# ============================
# COMPILER DEFINITIONS/LINKING
# ============================

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
        "$<BUILD_INTERFACE:${Vulkan_INCLUDE_DIR}>"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        spdlog::spdlog
        fmt::fmt
)
