add_executable(test_config_parameters_Matrix src/test_config_parameters_Matrix.cpp)
target_link_libraries(test_config_parameters_Matrix 
    PRIVATE 
        config
        Catch2::Catch2WithMain
        yaml-cpp 
)
target_include_directories(test_config_parameters_Matrix
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)
add_test(NAME test_config_parameters_Matrix COMMAND test_config_parameters_Matrix)