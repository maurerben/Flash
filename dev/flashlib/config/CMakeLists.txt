add_library(config  INTERFACE)

target_link_libraries(config 
        INTERFACE 
            constants
            Eigen3::Eigen
            yaml-cpp
)

target_include_directories(config
        INTERFACE
            include
)