set(CRYSTALGUI_EXAMPLES
)

foreach(EXAMPLE ${CRYSTALGUI_EXAMPLES})
    set(EXAMPLE_TARGET CrystalGUI_${EXAMPLE}_example)
    add_executable(${EXAMPLE_TARGET} ${EXAMPLE}.cpp)
    target_link_libraries(${EXAMPLE_TARGET} PRIVATE CrystalGUI)
    target_include_directories(${EXAMPLE_TARGET} PRIVATE ${CRYSTALGUI_SOURCE_DIR}/example)
endforeach()
