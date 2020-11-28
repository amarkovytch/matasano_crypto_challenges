# Add executable called "set2_challenge14" that is built from the source files
# "main.cpp". The extensions are automatically found.
add_executable (set2_challenge14 main.cpp)

# Link the executable to the utils library. Since the utils library has
# public include directories we will use those link directories when building
# set2_challenge14
target_link_libraries (set2_challenge14 LINK_PUBLIC utils)

