add_library(configuration  INTERFACE)

target_link_libraries(configuration 
        INTERFACE 
            basic
            Eigen3::Eigen
            yaml-cpp
)

target_include_directories(configuration
        INTERFACE
            include
)