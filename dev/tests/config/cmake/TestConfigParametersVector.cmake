add_executable(test_config_parameters_Vector src/test_config_parameters_Vector.cpp)
target_link_libraries(test_config_parameters_Vector 
    PRIVATE 
        config
        Catch2::Catch2WithMain
        yaml-cpp 
)
target_include_directories(test_config_parameters_Vector
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)
add_test(NAME test_config_parameters_Vector COMMAND test_config_parameters_Vector)