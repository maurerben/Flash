add_library(constants INTERFACE)

target_link_libraries(constants 
        INTERFACE 
            Eigen3::Eigen
)

target_include_directories(constants
        INTERFACE
            include
)