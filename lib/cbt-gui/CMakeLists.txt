include(${CMAKE_CURRENT_LIST_DIR}/cbt_gui_sources.cmake)

add_library(cbt-gui STATIC ${CBT_GUI_SOURCES} ${CBT_GUI_HEADERS})

target_include_directories(cbt-gui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

# vcpkg builds the wide-character ncurses variant (libncursesw.a).
# CURSES_NEED_WIDE tells FindCurses to search for "ncursesw" instead of "ncurses".
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)

target_include_directories(cbt-gui PRIVATE ${CURSES_INCLUDE_DIRS})

# Link to core cbt (GUI depends on it) and ncurses
target_link_libraries(cbt-gui PUBLIC
    cbt::cbt
    ${CURSES_LIBRARIES}
)

add_library(cbt::gui ALIAS cbt-gui)

install(TARGETS cbt-gui
    LIBRARY DESTINATION lib
)
