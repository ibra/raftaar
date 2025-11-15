cmake_minimum_required(VERSION 3.16)
project(raftaar LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui.git
  GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(sfml)

file(GLOB CORE_SOURCES "src/core/*.cpp")
file(GLOB DATA_SOURCES "src/data/*.cpp")
file(GLOB MODE_SOURCES "src/modes/*.cpp")
file(GLOB UI_SOURCES "src/ui/*.cpp")
file(GLOB UTIL_SOURCES "src/utils/*.cpp")

set(SOURCES
    src/main.cpp
    ${CORE_SOURCES}
    ${DATA_SOURCES}
    ${MODE_SOURCES}
    ${UI_SOURCES}
    ${UTIL_SOURCES}
)

add_executable(raftaar ${SOURCES})

target_include_directories(raftaar PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(raftaar
  PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
    sfml-graphics
    sfml-window
    sfml-system
)

add_custom_command(TARGET raftaar POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets
          ${CMAKE_CURRENT_BINARY_DIR}/assets
)