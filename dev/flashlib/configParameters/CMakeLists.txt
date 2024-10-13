add_library(configParameters  INTERFACE)

target_include_directories(configParameters
        INTERFACE
            include
)

target_link_libraries(configParameters 
        INTERFACE 
            basic
            yaml-cpp
)