if(NOT "${CRYSTALGUI_SOURCE_DIR}/documentation" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    find_package(Doxygen QUIET)
    if(Doxygen_FOUND)
        configure_file(
            "${CRYSTALGUI_SOURCE_DIR}/documentation/Doxyfile.in"
            "${CRYSTALGUI_BINARY_DIR}/documentation/Doxyfile"
            @ONLY
        )
        add_custom_target(documentation
            COMMAND ${CMAKE_COMMAND} -E echo "Generating documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} "${CRYSTALGUI_BINARY_DIR}/Doxyfile"
            WORKING_DIRECTORY "${CRYSTALGUI_SOURCE_DIR}"
            COMMENT "Generate API documentation with Doxygen"
            VERBATIM
        )
    endif()
endif()
