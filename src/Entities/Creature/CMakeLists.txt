target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(${EXECUTABLE_NAME} PRIVATE Creature.cc Creature.h)
target_sources(${EXECUTABLE_NAME} PRIVATE Trader.cc Trader.h)
