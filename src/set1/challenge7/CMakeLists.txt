# Add executable called "set1_challenge1" that is built from the source files
# "main.cpp". The extensions are automatically found.
add_executable (set1_challenge7 main.cpp)

# Link the executable to the utils library. Since the utils library has
# public include directories we will use those link directories when building
# set1_challenge2
target_link_libraries (set1_challenge7 LINK_PUBLIC utils cryptopp)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin)
