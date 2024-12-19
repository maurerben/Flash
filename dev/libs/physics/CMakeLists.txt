add_library(physics 
        src/ElectronicStates.cpp
)

target_link_libraries(physics
        PUBLIC
            data_processing
            constants
            config
)

target_include_directories(physics
        PUBLIC
            include
        PRIVATE
            src
)