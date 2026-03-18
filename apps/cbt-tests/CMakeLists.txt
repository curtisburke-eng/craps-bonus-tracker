include(${CMAKE_CURRENT_LIST_DIR}/cbt_tests_sources.cmake)

add_executable(cbt-tests ${CBT_TEST_SOURCES} ${CBT_TEST_HEADERS})

target_compile_definitions(cbt-tests PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

target_include_directories(cbt-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(GTest REQUIRED)

target_link_libraries(cbt-tests PRIVATE
    cbt::cbt
    spdlog::spdlog
    GTest::GTest
)

set_target_properties(cbt-tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(CTest)
add_test(NAME cbt-tests COMMAND cbt-tests)

install(TARGETS cbt-tests
    RUNTIME DESTINATION bin
)