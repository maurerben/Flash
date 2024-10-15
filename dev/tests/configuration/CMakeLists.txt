# Add the test executable
add_executable(configuration_tests src/)

# Collect all .cpp files in the 'src' directory
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Add the collected source files to the target
target_sources(configuration_tests PRIVATE ${SOURCE_FILES})

# Link the library and Catch2 to the test executable
target_link_libraries(configuration_tests 
    PRIVATE 
        configuration
        Catch2::Catch2WithMain
        yaml-cpp 
)

target_include_directories(configuration_tests
    PRIVATE
        src
        ${catch2_SOURCE_DIR}/include
)


# Add the tests to CTest
add_test(NAME test_configParameters COMMAND configuration_tests)