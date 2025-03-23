add_executable(test_physics_electrons_Index src/test_physics_electrons_Index.cpp)
target_link_libraries(test_physics_electrons_Index
    PRIVATE
        physics
        dataProcessing
        Catch2::Catch2WithMain
)
target_include_directories(test_physics_electrons_Index
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)
add_test(NAME test_physics_electrons_Index COMMAND test_physics_electrons_Index)
