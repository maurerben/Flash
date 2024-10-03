# Add the test executable
add_executable(data_processing_tests)

# Collect all .cpp files in the 'src' directory
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Add the collected source files to the target
target_sources(data_processing_tests PRIVATE ${SOURCE_FILES})

# Link the library and Catch2 to the test executable
target_link_libraries(data_processing_tests 
    PRIVATE 
        Catch2::Catch2WithMain
        data_processing 
        yaml-cpp 
)

target_include_directories(data_processing_tests
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)


# Add the tests to CTest
add_test(NAME data_processing_test COMMAND data_processing_tests)