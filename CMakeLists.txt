cmake_minimum_required(VERSION 3.0.0)
project(matsano VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -pedantic -Werror)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory (src/set1)
add_subdirectory (src/set1/challenge1)
add_subdirectory (src/set1/challenge2)
add_subdirectory (src/set1/challenge3)
add_subdirectory (src/set1/challenge4)
add_subdirectory (src/set1/challenge5)
add_subdirectory (src/set1/challenge6)
add_subdirectory (src/utils)
add_subdirectory (tests)

