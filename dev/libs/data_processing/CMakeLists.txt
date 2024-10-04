#project(lightning-config)

add_library(data_processing 
        src/config.cpp 
        src/read_hdf5.cpp
)

target_link_libraries(data_processing 
        types  
        yaml-cpp
)

target_include_directories(data_processing
        PUBLIC
            ${CMAKE_SOURCE_DIR}/include
        PRIVATE
            src
)