add_library(data_processing 
        src/hdf5.cpp
)

target_link_libraries(data_processing 
        PRIVATE
            constants 
            config
            HDF5::HDF5
)

target_include_directories(data_processing
        PUBLIC
            include
        PRIVATE
            src
)