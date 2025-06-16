add_library(utils
        src/IndexMap.cpp
)

target_include_directories(utils
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(utils PRIVATE USE_TBB)
target_link_libraries(utils
        PUBLIC
            HDF5::HDF5
            Eigen3::Eigen
            TBB::tbb

)

install(TARGETS utils EXPORT flashlightTargets)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include)