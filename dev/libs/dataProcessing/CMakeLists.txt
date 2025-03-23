add_library(dataProcessing
        src/utils.cpp
        src/access.cpp
)

target_link_libraries(dataProcessing
        PUBLIC
            utils
            HDF5::HDF5
            Eigen3::Eigen
)

target_include_directories(dataProcessing
        PUBLIC
            include
        PRIVATE
            src
)