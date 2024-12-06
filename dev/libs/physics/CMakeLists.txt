add_library(physics 
        src/ElectronicStates.cpp
)

target_link_libraries(physics 
        PRIVATE
            constants 
            config
            data_processing
)

target_include_directories(physics
        PUBLIC
            include
            data_processing
        PRIVATE
            src
)