#project(lightning-config)

add_library(config src/config.cpp)
target_link_libraries(config yaml-cpp types)

target_include_directories(config
        PUBLIC
            include
        PRIVATE
            src
)