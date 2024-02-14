cmake_minimum_required(VERSION 3.17)
project(GoodGame VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIBRARY_NAME_GAME "lib_game")
set(LIBRARY_NAME_PLAYER "lib_player")
set(EXECUTABLE_NAME "main")

add_subdirectory(app)
add_subdirectory(src)
