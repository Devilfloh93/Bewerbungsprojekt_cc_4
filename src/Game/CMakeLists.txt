set(LIBRARY_SOURCES "Game.cc")
set(LIBRARY_HEADERS "Game.h")

add_library(${LIBRARY_NAME_GAME} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${LIBRARY_NAME_GAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
    ${LIBRARY_NAME_GAME}
    PUBLIC sfml-graphics)
