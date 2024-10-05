# include(FetchContent)

# # Declare the Eigen dependency
# FetchContent_Declare(
#   eigen
#   GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
#   GIT_TAG 3.4.0 # Specify the Eigen version you want to use
# )

# # Make the content available
# FetchContent_MakeAvailable(eigen)

# # Optionally, include Armadillo headers if needed explicitly
# include_directories(${ARMADILLO_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})