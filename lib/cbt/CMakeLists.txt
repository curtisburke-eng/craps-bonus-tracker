include(${CMAKE_CURRENT_LIST_DIR}/cbt_sources.cmake)

add_library(cbt STATIC ${CBT_SOURCES} ${CBT_HEADERS})

target_include_directories(cbt PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

target_compile_definitions(cbt PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

target_compile_features(cbt PRIVATE cxx_std_20)

add_library(cbt::cbt ALIAS cbt)

target_link_libraries(cbt PUBLIC
    spdlog::spdlog
)

install(TARGETS cbt
    LIBRARY DESTINATION lib
)