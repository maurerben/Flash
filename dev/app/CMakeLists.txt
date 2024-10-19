project(lightning-app)

add_executable(app src/main.cpp)
target_link_libraries(app 
    PRIVATE 
        constants
        config
        data_processing 
        yaml-cpp
        ${Boost_LIBRARIES}
)
