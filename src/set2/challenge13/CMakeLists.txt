# Add executable called "set2_challenge13" that is built from the source files
# "main.cpp". The extensions are automatically found.
add_executable (set2_challenge13 main.cpp)

# Link the executable to the utils library. Since the utils library has
# public include directories we will use those link directories when building
# set2_challenge13
target_link_libraries (set2_challenge13 LINK_PUBLIC utils)

