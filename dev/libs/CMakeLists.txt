add_library(flashlight INTERFACE)

# Set a dedicated module output directory
set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

add_subdirectory(constants)
add_subdirectory(utils)
add_subdirectory(dataProcessing)
add_subdirectory(config)
add_subdirectory(physics)


 target_link_libraries(flashlight
         INTERFACE
             constants
             utils
             dataProcessing
             dataProcessing_F_interface
             config
             physics
 )

# Forward include dirs
target_include_directories(flashlight
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
)


install(EXPORT flashlightTargets
        FILE flashlightTargets.cmake
        NAMESPACE flashlight::
        DESTINATION lib/cmake/flashlight)

install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/flashlightConfig.cmake
        DESTINATION lib/cmake/flashlight)





