target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(${EXECUTABLE_NAME} PRIVATE Message.cc Message.h)
target_sources(${EXECUTABLE_NAME} PRIVATE MessageFormat.cc MessageFormat.h)
