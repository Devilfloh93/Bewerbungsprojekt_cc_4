cmake_minimum_required(VERSION 3.20)
project(NewProject LANGUAGES CXX)

set(EXECUTABLE_NAME "main")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(nlohmann_json)

# Necessary for shared library on Windows
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_executable(${EXECUTABLE_NAME})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME "GoodGame") #LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
target_sources(${EXECUTABLE_NAME} PRIVATE "./app/main.cc")
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_20)
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${PROJECT_SOURCE_DIR}")
target_link_libraries(${EXECUTABLE_NAME} PUBLIC sfml-graphics nlohmann_json)
add_subdirectory("src")

install(TARGETS ${EXECUTABLE_NAME})
