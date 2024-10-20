add_executable(test_config_parameters_Scalar src/test_config_parameters_Scalar.cpp)
target_link_libraries(test_config_parameters_Scalar 
    PRIVATE 
        config
        Catch2::Catch2WithMain
        yaml-cpp 
)
target_include_directories(test_config_parameters_Scalar
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)
add_test(NAME test_config_parameters_Scalar COMMAND test_config_parameters_Scalar)