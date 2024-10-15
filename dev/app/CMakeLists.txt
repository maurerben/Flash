project(lightning-app)

add_executable(app src/main.cpp)
target_link_libraries(app 
    PRIVATE 
        basic
        configuration
        data_processing 
        yaml-cpp
        ${Boost_LIBRARIES}
)
