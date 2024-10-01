# cmake/ConfigCatch2.cmake

# include(FetchContent)

# FetchContent_Declare(
#   catch2
#   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#   GIT_TAG v3.0.1 # Replace with the latest stable release
# )

# FetchContent_MakeAvailable(catch2)

# # This ensures that Catch2's target is available in the parent scope
# set(target_include_directories 
#     ${catch2_SOURCE_DIR}/include 
#     CACHE PATH 
#         "Path to Catch2 include directory" 
#     FORCE)

find_package(Catch2 REQUIRED)