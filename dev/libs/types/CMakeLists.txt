#project(lightning-types)

add_library(types src/types.cpp)

target_link_libraries(types yaml-cpp armadillo)

target_include_directories(types
        PUBLIC
            include
        PRIVATE
            src
)