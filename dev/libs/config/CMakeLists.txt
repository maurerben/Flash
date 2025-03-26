add_library(config  INTERFACE)

target_include_directories(constants INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(config 
        INTERFACE 
            constants
            Eigen3::Eigen
            yaml-cpp
)

install(TARGETS config EXPORT flashlightTargets)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include)