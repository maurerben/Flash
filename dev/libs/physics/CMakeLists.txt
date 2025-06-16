add_library(physics
        src/Algorithms.cpp
)

target_include_directories(physics
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(physics
        PUBLIC
            utils
            dataProcessing
            HDF5::HDF5
            Eigen3::Eigen
)

install(TARGETS physics EXPORT flashlightTargets)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include)