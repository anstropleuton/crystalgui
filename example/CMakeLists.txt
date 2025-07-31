set(CRYSTALGUI_EXAMPLES
    control_test_suite
)

foreach(EXAMPLE ${CRYSTALGUI_EXAMPLES})
    set(EXAMPLE_TARGET CrystalGUI_${EXAMPLE}_example)
    add_executable(${EXAMPLE_TARGET} ${EXAMPLE}.c)

    target_compile_options(${EXAMPLE_TARGET} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
        $<$<CXX_COMPILER_ID:MSVC>:/W3>
    )

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${EXAMPLE_TARGET} PRIVATE
            $<$<CXX_COMPILER_ID:GNU>:-fsanitize=address,leak,undefined>
            $<$<CXX_COMPILER_ID:Clang>:-fsanitize=address,leak,undefined>
            $<$<CXX_COMPILER_ID:MSVC>:/fsanitize=address,/fsanitize=leak,/fsanitize=undefined>
        )

        target_link_options(${EXAMPLE_TARGET} PRIVATE
            $<$<CXX_COMPILER_ID:GNU>:-fsanitize=address,leak,undefined>
            $<$<CXX_COMPILER_ID:Clang>:-fsanitize=address,leak,undefined>
            $<$<CXX_COMPILER_ID:MSVC>:/fsanitize=address,/fsanitize=leak,/fsanitize=undefined>
        )
    endif()

    target_link_libraries(${EXAMPLE_TARGET} PRIVATE CrystalGUI)
    target_include_directories(${EXAMPLE_TARGET} PRIVATE ${CRYSTALGUI_SOURCE_DIR}/example)
endforeach()
