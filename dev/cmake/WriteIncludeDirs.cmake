# Get the list of include directories (e.g., target_include_directories or include_directories)
get_property(INCLUDE_DIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)

# Specify the output file
set(OUTPUT_FILE "${CMAKE_BINARY_DIR}/include_paths.txt")

# Open the file for writing
file(WRITE ${OUTPUT_FILE} "")

# Iterate over the list of include directories and write each to the file
foreach(INCLUDE_DIR ${INCLUDE_DIRS})
    file(APPEND ${OUTPUT_FILE} "${INCLUDE_DIR}\n")
endforeach()