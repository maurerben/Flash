#project(lightning-config)

add_library(config src/config.cpp)
target_link_libraries(config PRIVATE yaml-cpp)

target_include_directories(config
        PUBLIC
            include
        PRIVATE
            src
            ${yaml-cpp_SOURCE_DIR}/include
)