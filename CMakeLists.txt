# Your project's CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(simple_blog)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Enable submodule checking for all libraries
set(GIT_SUBMODULE ON CACHE BOOL "Enable submodule checking" FORCE)
set(GIT_SUBMODULE_UPDATE_LATEST ON CACHE BOOL "Enable submodule updates" FORCE)


# Disable submodule checking for all libraries
# set(GIT_SUBMODULE OFF CACHE BOOL "Disable submodule checking" FORCE)
# set(GIT_SUBMODULE_UPDATE_LATEST OFF CACHE BOOL "Disable submodule updates" FORCE)

# Find Git package for submodule handling
find_package(Git QUIET)

# Initialize and update git submodules to latest versions


if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    option(GIT_SUBMODULE_UPDATE_LATEST "Update submodules to latest remote commits" ON)
    

    # option(GIT_SUBMODULE OFF)
    # option(GIT_SUBMODULE_UPDATE_LATEST OFF)

    if(GIT_SUBMODULE)
        message(STATUS "Initializing and updating submodules...")
        
        # First, initialize submodules if they don't exist
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_INIT_RESULT)
        if(NOT GIT_SUBMOD_INIT_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_INIT_RESULT}, please checkout submodules")
        endif()
        
        # If enabled, update submodules to latest remote commits
        if(GIT_SUBMODULE_UPDATE_LATEST)
            message(STATUS "Updating submodules to latest remote commits...")
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --remote --recursive
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            RESULT_VARIABLE GIT_SUBMOD_UPDATE_RESULT)
            if(NOT GIT_SUBMOD_UPDATE_RESULT EQUAL "0")
                message(WARNING "git submodule update --remote --recursive failed with ${GIT_SUBMOD_UPDATE_RESULT}, continuing with current submodule versions")
            else()
                message(STATUS "Submodules updated to latest versions successfully")
            endif()
        endif()
    endif()
endif()



# Temporarily suppress all warnings for the library build
set(CMAKE_CXX_FLAGS_BACKUP "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")  # Suppress all warnings for library

add_subdirectory("library")

# Restore original flags for main project
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -fsanitize=address -g -O0")
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(GLOB SRC_FILES controllers/*.cpp  models/*.cpp middlewares/*.cpp routes/*.cpp)

# Create executable
add_executable(simple_blog main.cpp ${SRC_FILES})

# Define ABSPATH preprocessor variable with the absolute path
target_compile_definitions(simple_blog PRIVATE CPP_PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/")

# Include directories for the library headers (this will also suppress warnings from headers)
target_include_directories(simple_blog SYSTEM PRIVATE library/)

# Add additional compiler flags to suppress warnings from library headers

# target_compile_options(simple_blog PRIVATE -Wno-comment -Wno-overloaded-virtual -Wno-reorder)

# Link against the main library
# The library's CMakeLists.txt handles linking all dependencies automatically
target_link_libraries(simple_blog hh_web_framework)

