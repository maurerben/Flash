#project(lightning-types)

add_library(types src/types.cpp)

target_link_libraries(types Eigen3::Eigen)

target_include_directories(types
        PUBLIC
            include
        PRIVATE
            src
)