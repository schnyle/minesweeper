cmake_minimum_required(VERSION 3.18)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra)

if(WIN32)
  set(SDL2_DIR "/usr/x86_64-w64-mingw32/lib/cmake/SDL2")
  set(SDL2_TTF_DIR "/usr/x86_64-w64-mingw32/lib/cmake/SDL2_ttf")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(SOURCES
  src/BufferCompositor/BufferCompositor.cpp
  src/BufferCompositor/FaceCompositor.cpp
  src/BufferCompositor/HeaderCompositor.cpp
  src/BufferCompositor/MinefieldElementCompositor.cpp
  src/GameLoop.cpp
  src/main.cpp
  src/Minesweeper.cpp
  src/SpriteFactory.cpp
  src/utils.cpp
  src/window/GameWindow.cpp
  src/window/SettingsWindow.cpp
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

if(WIN32)
  add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
else()
  add_executable(${PROJECT_NAME} ${SOURCES})
endif()

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/window
  ${CMAKE_CURRENT_SOURCE_DIR}/include/BufferCompositor
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_BINARY_DIR}/generated
  ${SDL2_INCLUDE_DIRS}
  /usr/x86_64-w64-mingw32/include/SDL2
)

if(WIN32)
  target_link_directories(${PROJECT_NAME} PRIVATE
    /usr/x86_64-w64-mingw32/lib
  )

  target_link_libraries(${PROJECT_NAME}
    PRIVATE
    mingw32
    SDL2main
    SDL2
    SDL2_ttf
  )
else()
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
  )
endif()

add_dependencies(${PROJECT_NAME} generate_font_header)
