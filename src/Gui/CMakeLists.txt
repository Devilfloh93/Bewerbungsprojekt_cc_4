set(LIBRARY_SOURCES "Gui.cc")
set(LIBRARY_HEADERS "Gui.h")

add_library(${LIBRARY_NAME_GUI} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${LIBRARY_NAME_GUI} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
    ${LIBRARY_NAME_GUI}
    PUBLIC sfml-graphics
    ${LIBRARY_NAME_GAME})
