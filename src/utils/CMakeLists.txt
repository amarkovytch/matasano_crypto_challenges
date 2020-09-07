# Create a library called "utils" which includes the source file "utils.cpp".
# The extension is already found. Any number of sources could be listed here.
add_library (utils utils.cpp)

# Make sure the compiler can find include files for our utils library
# when other libraries or executables link to utils
target_include_directories (utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})