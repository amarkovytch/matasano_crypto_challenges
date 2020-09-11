cmake_minimum_required(VERSION 3.13)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)

# Add test cpp file
add_executable(utils_tests
    utils_tests.cpp
)

# Link test executable against gtest & gtest_main
target_link_libraries(utils_tests LINK_PUBLIC ${GTEST_BOTH_LIBRARIES} pthread gmock gtest utils)

add_test(
    NAME utils_tests
    COMMAND utils_tests
)