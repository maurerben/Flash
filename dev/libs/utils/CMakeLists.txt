add_library(utils
        src/IndexMap.cpp
)

target_link_libraries(utils
        PUBLIC
            HDF5::HDF5
            Eigen3::Eigen
)

target_include_directories(utils
        PUBLIC
            include
        PRIVATE
            src
)