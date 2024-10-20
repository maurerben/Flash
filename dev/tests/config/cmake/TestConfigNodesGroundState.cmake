add_executable(test_config_nodes_GroundState src/test_config_nodes_GroundState.cpp)
target_link_libraries(test_config_nodes_GroundState 
    PRIVATE 
        config
        Catch2::Catch2WithMain
        yaml-cpp 
)
target_include_directories(test_config_nodes_GroundState
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)
add_test(NAME test_config_nodes_GroundState COMMAND test_config_nodes_GroundState)