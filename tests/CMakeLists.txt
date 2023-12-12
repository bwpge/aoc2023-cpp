cmake_minimum_required(VERSION 3.27)
project(libaoc_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    "collections_test.cpp"
    "decode_test.cpp"
    "grid_test.cpp"
    "hash_test.cpp"
    "stringutils_test.cpp"
)

# ================
# COMPILER OPTIONS
# ================

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# ===============
# CONFIGURE GTEST
# ===============

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
find_package("GTest" CONFIG REQUIRED)

target_link_libraries(
    ${PROJECT_NAME}
    libaoc::libaoc
    GTest::gtest
    GTest::gtest_main
)

# add tests to cmake
include(GoogleTest)
gtest_add_tests(TARGET ${PROJECT_NAME})
