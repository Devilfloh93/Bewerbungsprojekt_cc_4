cmake_minimum_required(VERSION 3.17)
project(NewProject VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIBRARY_NAME_GAME "lib_game")
set(LIBRARY_NAME_PLAYER "lib_player")
set(LIBRARY_NAME_SURFACE "lib_surface")
set(LIBRARY_NAME_GUI "lib_gui")
set(EXECUTABLE_NAME "main")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_subdirectory(app)
add_subdirectory(src)
