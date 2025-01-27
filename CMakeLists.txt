cmake_minimum_required(VERSION 3.28)
project(minesweeper)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra)

find_package(SDL2 REQUIRED)

add_executable(minesweeper
  src/main.cpp
  src/Game.cpp
  src/Renderer.cpp
  src/SpriteFactory.cpp
  src/data.cpp
)

target_include_directories(minesweeper
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(minesweeper PRIVATE SDL2::SDL2)
