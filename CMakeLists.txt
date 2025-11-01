# This cmake build files was made using the public repositories of both of these libraries. 

cmake_minimum_required(VERSION 3.16)
project(raftaar LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# FTXTUI 
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui.git
  GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

# SFML 
FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(sfml)

# TARGET 
add_executable(raftaar src/main.cpp)

target_link_libraries(raftaar
  PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
    sfml-graphics
    sfml-window
    sfml-system
)

# FOR COPYING ASSETS AFTER BUILDING
add_custom_command(TARGET raftaar POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets
          ${CMAKE_CURRENT_BINARY_DIR}/assets)