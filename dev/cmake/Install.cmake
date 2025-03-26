message(STATUS "Installing from: ${CMAKE_CURRENT_SOURCE_DIR}/include")



# Export all components
install(TARGETS
        constants
        utils
        dataProcessing
        dataProcessing_F_interface
        config
        physics
        flashlight
        EXPORT flashlightTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)

## Install Fortran module files
#install(DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}/
#        DESTINATION include
#        FILES_MATCHING PATTERN "*.mod"
#)
#
## Install headers (optional if C++ public API)
#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
#        DESTINATION include
#)

## Export target info
#install(EXPORT flashlightTargets
#        FILE flashlightTargets.cmake
#        NAMESPACE flashlight::
#        DESTINATION lib/cmake/flashlight
#)