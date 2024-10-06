project(lightning-app)

add_executable(app src/main.cpp)
target_link_libraries(app 
    PRIVATE 
        data_processing 
        flash_constants
        yaml-cpp
        ${Boost_LIBRARIES}
)
