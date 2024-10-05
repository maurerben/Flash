# include(FetchContent)

# # Fetch yaml-cpp
# FetchContent_Declare(
#   yaml-cpp
#   GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
#   GIT_TAG yaml-cpp-0.7.0 # Specify the version tag here
# )

# FetchContent_MakeAvailable(yaml-cpp)

# set(target_include_directories
#     ${yaml-cpp_SOURCE_DIR}/include 
#     CACHE PATH 
#         "Path to yaml-cpp include directory" 
#     FORCE)


# #set(YAML-CPP_INCLUDE) ${catch2_SOURCE_DIR}/include CACHE PATH "Path to Catch2 include directory" FORCE)

# # Example of another dependency
# # FetchContent_Declare(
# #   some_dependency
# #   GIT_REPOSITORY https://github.com/some_dependency.git
# #   GIT_TAG v1.0.0
# # )
# # FetchContent_MakeAvailable(some_dependency)

find_package(yaml-cpp REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIRS})