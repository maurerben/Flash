add_executable(test_utils_IndexMap src/test_utils_IndexMap.cpp)
target_link_libraries(test_utils_IndexMap
        PRIVATE
        utils
        Catch2::Catch2WithMain
)
target_include_directories(test_utils_IndexMap
        PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)
add_test(NAME test_utils_IndexMap COMMAND test_utils_IndexMap)