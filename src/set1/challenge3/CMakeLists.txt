# Add executable called "set_challenge1" that is built from the source files
# "main.cpp". The extensions are automatically found.
add_executable (set_challenge3 main.cpp)

# Link the executable to the utils library. Since the utils library has
# public include directories we will use those link directories when building
# set_challenge2
target_link_libraries (set_challenge3 LINK_PUBLIC utils)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin)