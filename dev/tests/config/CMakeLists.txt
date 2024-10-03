# Add the test executable
add_executable(config_tests)

# Collect all .cpp files in the 'src' directory
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Add the collected source files to the target
target_sources(config_tests PRIVATE ${SOURCE_FILES})

# Link the library and Catch2 to the test executable
target_link_libraries(config_tests 
    PRIVATE 
        config 
        yaml-cpp 
        Catch2::Catch2WithMain
)

target_include_directories(config_tests
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)


# Add the tests to CTest
add_test(NAME config_test COMMAND config_tests)