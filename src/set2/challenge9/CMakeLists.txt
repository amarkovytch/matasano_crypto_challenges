# Add executable called "set2_challenge9" that is built from the source files
# "main.cpp". The extensions are automatically found.
add_executable (set2_challenge9 main.cpp)

# Link the executable to the utils library. Since the utils library has
# public include directories we will use those link directories when building
# set2_challenge9
target_link_libraries (set2_challenge9 LINK_PUBLIC utils)
