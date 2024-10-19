# Add the test executable for test_config_parameters_Scalar
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

# Add the test executable for test_config_parameters_Option
add_executable(test_config_parameters_Option src/test_config_parameters_Option.cpp)
target_link_libraries(test_config_parameters_Option 
    PRIVATE 
        config
        Catch2::Catch2WithMain
        yaml-cpp 
)
target_include_directories(test_config_parameters_Option
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)
add_test(NAME test_config_parameters_Option COMMAND test_config_parameters_Option)

# Add the test executable for test_config_parameters_Vector
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

# Add the test executable for test_config_parameters_Matrix
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