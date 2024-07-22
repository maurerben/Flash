#project(lightning-config)

add_library(electrons src/electrons.cpp)

target_link_libraries(electrons 
        PRIVATE 
            Eigen3::Eigen
            types)

target_include_directories(electrons
        PUBLIC
            include
        PRIVATE
            src
)