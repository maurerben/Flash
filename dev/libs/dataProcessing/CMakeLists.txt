add_library(dataProcessing
        src/access.cpp
)
set_target_properties(dataProcessing PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(dataProcessing
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(dataProcessing
        PUBLIC
            utils
            HDF5::HDF5
            Eigen3::Eigen
)

install(TARGETS dataProcessing EXPORT flashlightTargets)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include)


# Include fortran interface
add_subdirectory(fortran_interface)