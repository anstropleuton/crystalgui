cmake_minimum_required(VERSION 3.15)

project(CrystalGUI
    DESCRIPTION "Crystal GUI is a GUI framework for raylib."
    VERSION 0.0.1
    LANGUAGES C)

set(CRYSTALGUI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CRYSTALGUI_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(BUILD_TESTS "Build tests" ON)
    option(BUILD_EXAMPLES "Build examples" ON)
    option(BUILD_DOCUMENTATION "Build documentations" ON)
    option(BUILD_PACKAGE "Build package" ON)
else()
    option(BUILD_TESTS "Build tests" OFF)
    option(BUILD_EXAMPLES "Build examples" OFF)
    option(BUILD_DOCUMENTATION "Build documentations" OFF)
    option(BUILD_PACKAGE "Build package" OFF)
endif()

find_program(IWYU_PATH NAMES include-what-you-use iwyu)
if(IWYU_PATH)
    set(IWYU_MAPPING "${CRYSTALGUI_SOURCE_DIR}/.iwyu.imp")
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${IWYU_PATH};-Xiwyu;--mapping_file=${IWYU_MAPPING}")
endif()

add_subdirectory(source)

if(BUILD_PACKAGE)
    include(GNUInstallDirs)

    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
    set(CPACK_PACKAGE_VENDOR "Anstro Pleuton")
    set(CPACK_PACKAGE_CONTACT "Anstro Pleuton (https://github.com/anstropleuton)")
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
    set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/anstropleuton/crystalgui")
    set(CPACK_RESOURCE_FILE_LICENSE "${CRYSTALGUI_SOURCE_DIR}/license.md")
    set(CPACK_RESOURCE_FILE_README "${CRYSTALGUI_SOURCE_DIR}/readme.md")

    set(CPACK_GENERATOR "TGZ;ZIP")

    include(CPack)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(BUILD_DOCUMENTATION)
    add_subdirectory(documentation)
endif()

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    configure_file(
        "${CRYSTALGUI_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${CRYSTALGUI_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )
    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CRYSTALGUI_BINARY_DIR}/cmake_uninstall.cmake
    )
endif()
