add_executable(test_utils_assert src/test_utils_assert.cpp)
target_link_libraries(test_utils_assert
        PRIVATE
        utils
        Catch2::Catch2WithMain
)
target_include_directories(test_utils_assert
        PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)
add_test(NAME test_utils_assert COMMAND test_utils_assert)