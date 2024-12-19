add_library(data_processing 
        src/hdf5.cpp
)

target_link_libraries(data_processing 
        PUBLIC
            HDF5::HDF5
            Eigen3::Eigen
)

target_include_directories(data_processing
        PUBLIC
            include
        PRIVATE
            src
)