add_executable(test_config_nodes_Spectrum src/test_config_nodes_Spectrum.cpp)
target_link_libraries(test_config_nodes_Spectrum 
    PRIVATE 
        config
        Catch2::Catch2WithMain
        yaml-cpp 
)
target_include_directories(test_config_nodes_Spectrum
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)
add_test(NAME test_config_nodes_Spectrum COMMAND test_config_nodes_Spectrum)