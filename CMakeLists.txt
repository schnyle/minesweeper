cmake_minimum_required(VERSION 3.28)
project(minesweeper)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(SOURCES
  src/main.cpp
  src/Minesweeper.cpp
  src/window/GameWindow.cpp
  src/window/SettingsWindow.cpp
  src/GameLoop.cpp
  src/SpriteFactory.cpp
)

# setup font embedding
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated")
add_custom_command(
  OUTPUT "${CMAKE_BINARY_DIR}/generated/font.h"
  COMMAND xxd -i assets/UbuntuMono-B.ttf > "${CMAKE_BINARY_DIR}/generated/font.h"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  DEPENDS "${CMAKE_SOURCE_DIR}/assets/UbuntuMono-B.ttf"
)
add_custom_target(generate_font_header
  DEPENDS "${CMAKE_BINARY_DIR}/generated/font.h"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/window
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_BINARY_DIR}/generated
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  SDL2::SDL2
  SDL2_ttf::SDL2_ttf
)

add_dependencies(${PROJECT_NAME} generate_font_header)
