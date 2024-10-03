#project(lightning-config)

add_library(data_processing src/config.cpp)
target_link_libraries(data_processing yaml-cpp types)

target_include_directories(data_processing
        PUBLIC
            include
        PRIVATE
            src
)