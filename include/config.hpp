#pragma once

#include <cstdint>
#include <string>

namespace config
{
inline const char *APP_NAME = "Minesweeper";

constexpr uint32_t DARK_GREY = 0x7a7a7b;
constexpr uint32_t GREY = 0xbdbdbd;
constexpr uint32_t LIGHT_GREY = 0xfefffe;
constexpr uint32_t RED = 0xff0000;

constexpr int GRID_HEIGHT = 20; // must fit inside short - see Renderer::draw3DEdges
constexpr int GRID_WIDTH = 38;  // "                                               "

inline int WINDOW_PIXEL_HEIGHT = 1080;
inline int WINDOW_PIXEL_WIDTH = 1920;

constexpr int CELL_PIXEL_SIZE = 50;
constexpr int CELL_BORDER_WIDTH_3D = 5;
constexpr int CELL_BORDER_WIDTH_2D = 2;    // even int
constexpr size_t IMAGE_SIZE = 50 * 50 * 4; // images in assets/

constexpr int spriteSize = config::CELL_PIXEL_SIZE * config::CELL_PIXEL_SIZE * 4;
} // namespace config