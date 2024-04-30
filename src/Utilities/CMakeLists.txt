target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(${EXECUTABLE_NAME} PRIVATE Utilities.cc Utilities.h)
target_sources(${EXECUTABLE_NAME} PRIVATE Collision.cc Collision.h)
target_sources(${EXECUTABLE_NAME} PRIVATE Performance.cc Performance.h)
