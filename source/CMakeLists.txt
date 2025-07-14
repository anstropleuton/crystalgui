add_library(CrystalGUI
    cg_core.c
    cg_easings.c
    cg_element.c
    cg_extra.c
    cg_layout.c
    cg_node.c
)
target_include_directories(CrystalGUI PUBLIC
    $<BUILD_INTERFACE:${CRYSTALGUI_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CRYSTALGUI_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(CrystalGUI PUBLIC cxx_std_23)

install(TARGETS CrystalGUI
    EXPORT CrystalGUI_EXPORT
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY "${CRYSTALGUI_SOURCE_DIR}/include" DESTINATION "include")
install(EXPORT CrystalGUI_EXPORT
    FILE "CrystalGUI-config.cmake"
    NAMESPACE cgui::
    DESTINATION "lib/cmake/CrystalGUI"
)
export(TARGETS CrystalGUI
    FILE "${CRYSTALGUI_BINARY_DIR}/CrystalGUI_export.cmake"
    NAMESPACE cgui::
)

configure_file(
    "${CRYSTALGUI_SOURCE_DIR}/cmake/CrystalGUI.pc.in"
    "${CRYSTALGUI_BINARY_DIR}/cmake/CrystalGUI.pc"
    @ONLY
)
install(FILES "${CRYSTALGUI_BINARY_DIR}/cmake/CrystalGUI.pc" DESTINATION "lib/pkgconfig")
