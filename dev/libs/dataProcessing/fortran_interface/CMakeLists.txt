add_library(dataProcessing_F_interface SHARED
        dataProcessing_F_interface.cpp
        dataProcessing_F_interface.F90
)

target_include_directories(dataProcessing_F_interface
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
)

target_link_libraries(dataProcessing_F_interface
        PUBLIC
            utils
            dataProcessing
            HDF5::HDF5
            Eigen3::Eigen)

install(TARGETS dataProcessing_F_interface
        EXPORT flashlightTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}/
        DESTINATION include
        FILES_MATCHING PATTERN "*.mod")
