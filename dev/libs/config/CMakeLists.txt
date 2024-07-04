#project(lightning-config)

add_library(config src/config.cpp)
target_link_libraries(config yaml-cpp)

target_include_directories(config
        PUBLIC
            include
            ${yaml-cpp_SOURCE_DIR}/include
        PRIVATE
            src
)