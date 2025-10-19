cmake_minimum_required(VERSION 3.18)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(STATIC_BUILD "Build with static SDL2 libraries" OFF)

include(FetchContent)

if(STATIC_BUILD)
  set(BUILD_SHARED_LIBS OFF)
else()
  set(BUILD_SHARED_LIBS ON)
endif()

if(WIN32)
  set(SDL2_DIR "/usr/x86_64-w64-mingw32/lib/cmake/SDL2")
  set(SDL2_TTF_DIR "/usr/x86_64-w64-mingw32/lib/cmake/SDL2_ttf")
endif()

# temporarily disable warnings for SDL2 build
set(CMAKE_C_FLAGS_BACKUP ${CMAKE_C_FLAGS})
set(CMAKE_CXX_FLAGS_BACKUP ${CMAKE_CXX_FLAGS})
set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

if(STATIC_BUILD)
  set(SDL_PIPEWIRE OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.32.10
  )
  FetchContent_MakeAvailable(SDL2)

  FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-2.24.0
  )
  FetchContent_MakeAvailable(SDL2_ttf)
else()
  find_package(SDL2 REQUIRED)
  find_package(SDL2_ttf REQUIRED)
endif()

# restore warning flags
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS_BACKUP})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_BACKUP})
add_compile_options(-Wall -Wextra)

set(SOURCES
  src/Artist/BaseArtist.cpp
  src/Artist/FaceArtist.cpp
  src/Artist/HeaderArtist.cpp
  src/Artist/MinefieldArtist.cpp
  src/GameLoop.cpp
  src/main.cpp
  src/Minesweeper.cpp
  src/Sprites.cpp
  src/utils.cpp
  src/Window/GameWindow.cpp
  src/Window/SettingsWindow.cpp
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

if(STATIC_BUILD)
  target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${sdl2_SOURCE_DIR}/include
    ${sdl2_ttf_SOURCE_DIR}
  )
endif()

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Artist
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Window
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_BINARY_DIR}/generated
)

if(WIN32)
  target_link_directories(${PROJECT_NAME} PRIVATE
    /usr/x86_64-w64-mingw32/lib
  )

  target_link_libraries(${PROJECT_NAME}
    PRIVATE
    mingw32
    SDL2main
    SDL2-static
    SDL2_ttf-static
  )
else()
  if(STATIC_BUILD)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2-static SDL2_ttf)
  else()
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf)
  endif()
endif()

add_dependencies(${PROJECT_NAME} generate_font_header)
