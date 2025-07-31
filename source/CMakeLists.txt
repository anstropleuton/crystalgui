add_library(CrystalGUI
    cg_components.c
    cg_core.c
    cg_crystalline.c
    cg_easings.c
    cg_element.c
    cg_event.c
    cg_extra.c
    cg_layout.c
    cg_node.c
    cg_theme.c
    cg_transition.c
)
target_include_directories(CrystalGUI PUBLIC
    $<BUILD_INTERFACE:${CRYSTALGUI_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CRYSTALGUI_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(CrystalGUI PUBLIC c_std_99)

target_compile_options(CrystalGUI PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>:/W3>
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(CrystalGUI PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-fsanitize=address,leak,undefined>
        $<$<CXX_COMPILER_ID:Clang>:-fsanitize=address,leak,undefined>
        $<$<CXX_COMPILER_ID:MSVC>:/fsanitize=address,/fsanitize=leak,/fsanitize=undefined>
    )

    target_link_options(CrystalGUI PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-fsanitize=address,leak,undefined>
        $<$<CXX_COMPILER_ID:Clang>:-fsanitize=address,leak,undefined>
        $<$<CXX_COMPILER_ID:MSVC>:/fsanitize=address,/fsanitize=leak,/fsanitize=undefined>
    )
endif()

# Add dependencies
include("${CRYSTALGUI_SOURCE_DIR}/cmake/subdep.cmake")

set(BUILD_EXAMPLES OFF) # for raylib
subdep_add(raylib)

target_link_libraries(CrystalGUI PUBLIC raylib)

install(TARGETS CrystalGUI raylib glfw
    EXPORT CrystalGUI_EXPORT
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)
install(DIRECTORY "${CRYSTALGUI_SOURCE_DIR}/include" DESTINATION "include")
install(EXPORT CrystalGUI_EXPORT
    FILE "CrystalGUI-config.cmake"
    NAMESPACE cgui::
    DESTINATION "lib/cmake/CrystalGUI"
)
export(TARGETS CrystalGUI raylib glfw
    FILE "${CRYSTALGUI_BINARY_DIR}/CrystalGUI_export.cmake"
    NAMESPACE cgui::
)

configure_file(
    "${CRYSTALGUI_SOURCE_DIR}/cmake/CrystalGUI.pc.in"
    "${CRYSTALGUI_BINARY_DIR}/cmake/CrystalGUI.pc"
    @ONLY
)
install(FILES "${CRYSTALGUI_BINARY_DIR}/cmake/CrystalGUI.pc" DESTINATION "lib/pkgconfig")
