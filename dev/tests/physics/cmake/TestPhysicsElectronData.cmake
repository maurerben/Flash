add_executable(test_physics_ElectronData src/test_physics_ElectronData.cpp)
target_link_libraries(test_physics_ElectronData 
    PRIVATE
        dataProcessing
        utils
        physics
        Catch2::Catch2WithMain
)
target_include_directories(test_physics_ElectronData
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)
add_test(NAME test_physics_ElectronData COMMAND test_physics_ElectronData)