# Your project's CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(simple_blog)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Disable submodule checking for all libraries
set(GIT_SUBMODULE OFF CACHE BOOL "Disable submodule checking" FORCE)
set(GIT_SUBMODULE_UPDATE_LATEST OFF CACHE BOOL "Disable submodule updates" FORCE)

# Add the library subdirectory
# This will automatically build the library and all its dependencies
add_subdirectory(library)

# Create executable
add_executable(simple_blog main.cpp)

# Link against the main library
# The library's CMakeLists.txt handles linking all dependencies automatically
target_link_libraries(simple_blog hamza_web_framework)

