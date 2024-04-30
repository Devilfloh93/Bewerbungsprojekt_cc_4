target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(${EXECUTABLE_NAME} PRIVATE ItemGround.cc ItemGround.h)
target_sources(${EXECUTABLE_NAME} PRIVATE ItemCfg.cc ItemCfg.h)
