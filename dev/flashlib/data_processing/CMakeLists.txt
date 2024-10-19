add_library(data_processing 
        src/yaml.cpp 
        src/hdf5.cpp
)

target_link_libraries(data_processing 
        PRIVATE
            constants 
            yaml-cpp
)

target_include_directories(data_processing
        PUBLIC
            include
        PRIVATE
            src
)