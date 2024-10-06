#project(lightning-config)

add_library(data_processing 
        src/config.cpp 
        src/hdf5_wrappers.cpp
)

target_link_libraries(data_processing 
        PUBLIC
            types  
            yaml-cpp
)

target_include_directories(data_processing
        PUBLIC
            include
        PRIVATE
            src
)