cmake_minimum_required(VERSION 3.26)
project(neuron-fsm CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

add_library(neuron_fsm ${SOURCE_FILES})

target_include_directories(neuron_fsm PRIVATE include)

# Get version from Git
execute_process(
    COMMAND git describe --tags --abbrev=0
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE PROJECT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Fallback for when no tags are available
if (NOT PROJECT_VERSION)
    set(PROJECT_VERSION "0.0.0-unknown")
endif()

# Set library properties
set_target_properties(neuron_fsm PROPERTIES VERSION ${PROJECT_VERSION})
message(STATUS "Project Version: ${PROJECT_VERSION}")

option(BUILD_SHARED_LIBS "Build as a shared library" OFF)

# Add third-party libraries
add_subdirectory(thirdparty)

# Add an option to enable tests
# Tests are disabled (OFF) by default
# Use -DBUILD_TESTS=ON in the generation stage to enable them
option(BUILD_TESTS "Build tests" OFF)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
