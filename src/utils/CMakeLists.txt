list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
find_package(BOTAN REQUIRED)

file(GLOB_RECURSE source_list "*.cpp" "*.hpp")

# Create a library called "utils" which includes the source file "utils.cpp".
# The extension is already found. Any number of sources could be listed here.
add_library (utils ${source_list})

# Make sure the compiler can find include files for our utils library
# when other libraries or executables link to utils
target_include_directories (utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (utils PUBLIC ${BOTAN_INCLUDE_DIR})
