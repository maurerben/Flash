add_executable(test_physics_electrons_ElectronicStates src/test_physics_electrons_ElectronicStates.cpp)
target_link_libraries(test_physics_electrons_ElectronicStates 
    PRIVATE 
        physics
        Catch2::Catch2WithMain
)
target_include_directories(test_physics_electrons_ElectronicStates
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)
add_test(NAME test_physics_electrons_ElectronicStates COMMAND test_physics_electrons_ElectronicStates)