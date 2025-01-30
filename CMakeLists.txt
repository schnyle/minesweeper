cmake_minimum_required(VERSION 3.28)
project(minesweeper)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
set(PROJECT_ASSETS_DIR "${PROJECT_ROOT_DIR}/assets")

configure_file(
  "${PROJECT_ROOT_DIR}/cmake/paths.h.in"
  "${CMAKE_BINARY_DIR}/generated/paths.h"
)

set(SOURCES
  src/main.cpp
  src/Minesweeper.cpp
  src/Renderer.cpp
  src/GameLoop.cpp
  src/SpriteFactory.cpp
)

include_directories("${CMAKE_BINARY_DIR}/generated")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  $
)

target_link_libraries(minesweeper PRIVATE SDL2::SDL2)
target_link_libraries(minesweeper PRIVATE SDL2_ttf::SDL2_ttf)
