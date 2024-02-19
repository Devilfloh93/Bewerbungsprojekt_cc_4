set(EXE_SOURCES "${EXECUTABLE_NAME}.cc")
add_executable(${EXECUTABLE_NAME} ${EXE_SOURCES})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME "GoodGame") #LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")

target_link_libraries(
    ${EXECUTABLE_NAME}
    PUBLIC sfml-graphics
    ${LIBRARY_NAME_PLAYER}
    ${LIBRARY_NAME_GAME}
    ${LIBRARY_NAME_SURFACE})
