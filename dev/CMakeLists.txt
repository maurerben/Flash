cmake_minimum_required(VERSION 3.29)
project(lightning)

set(CMAKE_CXX_STANDARD 20)

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


# Include libraries
add_subdirectory(libs)

# Include application
add_subdirectory(app)

# Include tests
add_subdirectory(tests)

# Set compile options for different build types
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -tr")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")