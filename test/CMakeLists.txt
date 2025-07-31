include("${CRYSTALGUI_SOURCE_DIR}/cmake/subdep.cmake")

subdep_add(doctest)

set(CRYSTALGUI_TESTS
)

foreach(TEST ${CRYSTALGUI_TESTS})
    set(TEST_TARGET CrystalGUI_${TEST}_test)
    add_executable(${TEST_TARGET} ${TEST}.c)

    target_compile_options(${TEST_TARGET} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
        $<$<CXX_COMPILER_ID:MSVC>:/W3>
    )

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${TEST_TARGET} PRIVATE
            $<$<CXX_COMPILER_ID:GNU>:-fsanitize=address,leak,undefined>
            $<$<CXX_COMPILER_ID:Clang>:-fsanitize=address,leak,undefined>
            $<$<CXX_COMPILER_ID:MSVC>:/fsanitize=address,/fsanitize=leak,/fsanitize=undefined>
        )

        target_link_options(${TEST_TARGET} PRIVATE
            $<$<CXX_COMPILER_ID:GNU>:-fsanitize=address,leak,undefined>
            $<$<CXX_COMPILER_ID:Clang>:-fsanitize=address,leak,undefined>
            $<$<CXX_COMPILER_ID:MSVC>:/fsanitize=address,/fsanitize=leak,/fsanitize=undefined>
        )
    endif()

    target_link_libraries(${TEST_TARGET} PRIVATE CrystalGUI doctest::doctest)
    target_include_directories(${TEST_TARGET} PRIVATE ${CRYSTALGUI_SOURCE_DIR}/test)
    add_test(NAME ${TEST} COMMAND ${TEST_TARGET})
endforeach()
