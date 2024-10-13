add_library(basic INTERFACE)

target_link_libraries(basic 
        INTERFACE 
            Eigen3::Eigen
)

target_include_directories(basic
        INTERFACE
            include
)