# Specify the Boost version you want to use
find_package(Boost 1.70 REQUIRED COMPONENTS program_options)

# Include Boost headers
include_directories(${Boost_INCLUDE_DIRS})