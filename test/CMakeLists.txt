include("${CRYSTALGUI_SOURCE_DIR}/cmake/subdep.cmake")

subdep_add(doctest)

set(CRYSTALGUI_TESTS
)

foreach(TEST ${CRYSTALGUI_TESTS})
    set(TEST_TARGET CrystalGUI_${TEST}_test)
    add_executable(${TEST_TARGET} ${TEST}.cpp)
    target_link_libraries(${TEST_TARGET} PRIVATE CrystalGUI doctest::doctest)
    target_include_directories(${TEST_TARGET} PRIVATE ${CRYSTALGUI_SOURCE_DIR}/test)
    add_test(NAME ${TEST} COMMAND ${TEST_TARGET})
endforeach()
