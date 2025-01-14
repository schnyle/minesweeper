cmake_minimum_required(VERSION 3.28)
project(minesweeper)

add_compile_options(-Wall -Wextra)

find_package(X11 REQUIRED)

add_executable(minesweeper
  src/minesweeper.cpp
  src/Game.cpp
  src/Renderer.cpp
  src/data.cpp
)

target_include_directories(minesweeper
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(minesweeper PRIVATE X11)

add_custom_command(
  TARGET minesweeper
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/assets
  ${CMAKE_BINARY_DIR}/assets
)
