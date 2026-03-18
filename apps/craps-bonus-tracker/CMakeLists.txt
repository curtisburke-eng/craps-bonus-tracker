
include(${CMAKE_CURRENT_LIST_DIR}/app_sources.cmake)

add_executable(craps-bonus-tracker ${CBT_APP_SOURCES} ${CBT_APP_HEADERS})

target_link_libraries(craps-bonus-tracker PRIVATE
    cbt::cbt
    # cbt::gui
    spdlog::spdlog
)

set_target_properties(craps-bonus-tracker PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

install(TARGETS craps-bonus-tracker
    RUNTIME DESTINATION bin
)
