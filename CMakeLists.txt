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

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES 
    "src/main.cpp"
    "src/mode_*.cpp"
    "src/typing_state.cpp"
    "src/ui_components.cpp"
    "src/equation_generation.cpp"
)

add_executable(raftaar ${SOURCES})

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
          ${CMAKE_CURRENT_BINARY_DIR}/assets)
