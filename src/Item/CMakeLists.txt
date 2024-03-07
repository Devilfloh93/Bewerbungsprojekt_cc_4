set(LIBRARY_SOURCES "Item.cc")
set(LIBRARY_HEADERS "Item.h")

add_library(${LIBRARY_NAME_ITEM} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${LIBRARY_NAME_ITEM} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
    ${LIBRARY_NAME_ITEM}
    PUBLIC sfml-graphics
    nlohmann_json
    ${LIBRARY_NAME_GUI})
