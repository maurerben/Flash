#include(FetchContent)
#
# Fetch HDF5
#FetchContent_Declare(
#  hdf5
#  GIT_REPOSITORY https://github.com/HDFGroup/hdf5.git
#  GIT_TAG hdf5-1_14_4 # Specify the version tag here
#)
#
#FetchContent_MakeAvailable(hdf5)
#
#set(target_include_directories
#    ${HDF5_SOURCE_DIR}/include 
#    CACHE PATH 
#        "Path to hdf5 include directory" 
#    FORCE)

find_package(HDF5 REQUIRED COMPONENTS C CXX)
include_directories(${HDF5_INCLUDE_DIRS})