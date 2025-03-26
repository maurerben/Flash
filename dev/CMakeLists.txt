cmake_minimum_required(VERSION 3.29)
project(lightning)
project(ElectronsHDF5Fortran LANGUAGES C CXX Fortran)

# Set C++ standart
set(CMAKE_CXX_STANDARD 20)

#message(STATUS "🚧 CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
#message(STATUS "🚧 CMAKE_INSTALL_PREFIX     = ${CMAKE_INSTALL_PREFIX}")
#message(STATUS "🚧 PROJECT_INCLUDE_DIR      = ${CMAKE_CURRENT_SOURCE_DIR}/include")

# Generate a compile_commands.json
include(cmake/ConfigClangTidy.cmake)

# Enable testing
enable_testing()

# Include the Catch2 configuration file
include(cmake/ConfigCatch2.cmake)

# Include the HDF5 configuration file
include(cmake/ConfigHDF5.cmake)

# Include the yaml-cpp  configuration file
include(cmake/ConfigYamlCpp.cmake)

# Include the Eigen  configuration file
include(cmake/ConfigEigen.cmake)

# Include the Boost configuration file
include(cmake/ConfigBoost.cmake)

# Include libraries
add_subdirectory(libs)

# Include application
add_subdirectory(app)

# Include tests
add_subdirectory(tests)


# Set compile options for different build types
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -tr -fsanitize=address ")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Write include dirs to a file
#include(cmake/WriteIncludeDirs.cmake)

#include(cmake/Install.cmake)