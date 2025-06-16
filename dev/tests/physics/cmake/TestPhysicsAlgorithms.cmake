add_executable(test_physics_Algorithms src/test_physics_Algorithms.cpp)
target_link_libraries(test_physics_Algorithms 
    PRIVATE
        dataProcessing
        utils
        physics
        Catch2::Catch2WithMain
)
target_include_directories(test_physics_Algorithms
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)
add_test(NAME test_physics_Algorithms COMMAND test_physics_Algorithms)