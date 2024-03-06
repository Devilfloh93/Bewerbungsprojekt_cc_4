set(LIBRARY_SOURCES "Surface.cc")
set(LIBRARY_HEADERS "Surface.h")

add_library(${LIBRARY_NAME_SURFACE} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${LIBRARY_NAME_SURFACE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
    ${LIBRARY_NAME_SURFACE}
    PUBLIC sfml-graphics
    ${LIBRARY_NAME_PLAYER})
