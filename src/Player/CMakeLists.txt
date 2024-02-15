set(LIBRARY_SOURCES "Player.cc")
set(LIBRARY_HEADERS "Player.h")

add_library(${LIBRARY_NAME_PLAYER} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${LIBRARY_NAME_PLAYER} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
    ${LIBRARY_NAME_PLAYER}
    PUBLIC sfml-graphics)
