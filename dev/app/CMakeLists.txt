project(lightning-app)

add_executable(app src/main.cpp)
target_link_libraries(app 
    PRIVATE
        flashlight
        yaml-cpp
        HDF5::HDF5
        ${Boost_LIBRARIES}
)
