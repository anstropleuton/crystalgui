# Subordinate Dependency (subdep) is a CMake module for adding dependency from
# various sources, mainly Git Submodules.
#
# Dependencies are added from one of 4 sources, and each sources are checked
# individually based on their priority.
#
# 1. From custom path: Define a variable <DEPENDENCY_NAME>_PATH to add
#    dependency from that custom path.
# 2. From initialized submodule: Initialize a git submodule using
#    ```bash
#    git submodule init deps/dependency_name --update --recursive
#    ```
#    to add dependency from the git submodule path.
# 3. System-installed: Install dependency in your system to be used.
# 4. Initialize submodule: Automatically initialize a git submodule and add
#    dependency from the submodule path.

function(subdep_add DEP_NAME)
    string(TOUPPER "${DEP_NAME}" DEP_NAME_UPPER)

    # 1. Use custom path
    if(DEFINED "${DEP_NAME_UPPER}_PATH")
        message(STATUS "Adding dependency '${DEP_NAME}' from '${${DEP_NAME_UPPER}_PATH}' (specified by ${DEP_NAME_UPPER}_PATH)")
        add_subdirectory("${${DEP_NAME_UPPER}_PATH}" ${DEP_NAME})
        return()
    endif()

    # 2. Use initialized submodule
    if(EXISTS "${CRYSTALGUI_SOURCE_DIR}/deps/${DEP_NAME}/.git")
        message(STATUS "Adding dependency '${DEP_NAME}' from '${CRYSTALGUI_SOURCE_DIR}/deps/${DEP_NAME}' (initialized git submodule)")
        add_subdirectory("${CRYSTALGUI_SOURCE_DIR}/deps/${DEP_NAME}" ${DEP_NAME})
        return()
    endif()

    # 3. Use system-installed
    find_package(${DEP_NAME} QUIET)
    if(${DEP_NAME}_FOUND)
        message(STATUS "Adding dependency '${DEP_NAME}' (system-installed)")
        return()
    endif()

    # 4. Initialize git submodule
    message(STATUS "Initializing git submodule for '${DEP_NAME}' at '${CRYSTALGUI_SOURCE_DIR}/deps/${DEP_NAME}'")
    execute_process(
        COMMAND git submodule update --init --recursive --depth=1 "deps/${DEP_NAME}"
        WORKING_DIRECTORY ${CRYSTALGUI_SOURCE_DIR}
        RESULT_VARIABLE result
    )
    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Failed to initialize submodule for '${DEP_NAME}'")
    endif()
    add_subdirectory("${CRYSTALGUI_SOURCE_DIR}/deps/${DEP_NAME}" ${DEP_NAME})
endfunction()
