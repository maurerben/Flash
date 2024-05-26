cmake_minimum_required(VERSION 3.29)
project(lightning)

set(CMAKE_CXX_STANDARD 20)

# Enable testing
enable_testing()

# Include the Catch2 configuration file
include(cmake/ConfigCatch2.cmake)

# Include the YamlCpp.cmake
include(cmake/ConfigYamlCpp.cmake)

# Include libraries
add_subdirectory(libs)

# Include application
add_subdirectory(app)

# Include tests
add_subdirectory(tests)