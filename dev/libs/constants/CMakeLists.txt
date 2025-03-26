add_library(constants INTERFACE)

target_include_directories(constants INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(constants 
        INTERFACE 
            Eigen3::Eigen
)

install(TARGETS constants EXPORT flashlightTargets)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include)