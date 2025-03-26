# flashlightConfig.cmake
find_package(HDF5 REQUIRED COMPONENTS C CXX)
find_package(Eigen3 REQUIRED)
include("${CMAKE_CURRENT_LIST_DIR}/flashlightTargets.cmake")