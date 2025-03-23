project(lightning-app)

add_executable(app src/main.cpp)
target_link_libraries(app 
    PRIVATE 
        constants
        utils
        config
        dataProcessing
        physics
        yaml-cpp
        HDF5::HDF5
        ${Boost_LIBRARIES}
)
