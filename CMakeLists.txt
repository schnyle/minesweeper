cmake_minimum_required(VERSION 4.0)
project(minesweeper LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# TODO: move warnings onto our own targets (target_compile_options PRIVATE) so -Werror doesn't
#       reach fetched dependencies (gtest, SDL); mark deps SYSTEM; consider -Werror via preset/CI
add_compile_options(-Wall -Wextra -Werror -Wpedantic)

find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3-shared)

add_subdirectory(src/engine)
add_subdirectory(src/gui)

add_executable(minesweeper src/main.cpp)
target_link_libraries(minesweeper PRIVATE SDL3::SDL3)
target_link_libraries(minesweeper PRIVATE gui engine)


find_program(CLANG_TIDY_EXE clang-tidy)
if (CLANG_TIDY_EXE)
  set_target_properties(minesweeper engine gui PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
else()
  message(STATUS "clang-tidy not found; skipping")
endif()


enable_testing()
add_subdirectory(tests)
