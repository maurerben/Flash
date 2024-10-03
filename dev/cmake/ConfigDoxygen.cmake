# Find Doxygen package
find_package(Doxygen REQUIRED)

# Set Doxygen input and output paths
set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/DoxygenDocs)

# Create a custom command to run Doxygen
add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )

# Create a custom target for the documentation
add_custom_target(doc ALL DEPENDS ${DOXYGEN_OUT})