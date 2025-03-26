get_target_property(RAW_INCLUDE_DIRS dataProcessing_F_interface INCLUDE_DIRECTORIES)

set(OUTPUT_FILE "${CMAKE_BINARY_DIR}/include_paths.txt")
file(WRITE ${OUTPUT_FILE} "")

foreach(DIR ${RAW_INCLUDE_DIRS})
    if(DIR MATCHES "\\$<BUILD_INTERFACE:(.*)>")
        string(REGEX REPLACE "\\$<BUILD_INTERFACE:(.*)>" "\\1" CLEANED_DIR "${DIR}")
        file(APPEND ${OUTPUT_FILE} "${CLEANED_DIR}\n")
    endif()
endforeach()