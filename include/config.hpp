#pragma once

#include <cstdint>
#include <string>

namespace config
{
inline const char *APP_NAME = "Minesweeper";

constexpr uint32_t BLACK = 0xff000000;
constexpr uint32_t BLUE = 0xff0000ff;
constexpr uint32_t DARK_BLUE = 0x01007a;
constexpr uint32_t DARK_GREY = 0x7a7a7b;
constexpr uint32_t DARK_RED = 0x840101;
constexpr uint32_t GREEN = 0xff017a00;
constexpr uint32_t GREY = 0xbdbdbd;
constexpr uint32_t LIGHT_GREY = 0xfefffe;
constexpr uint32_t PURPLE = 0x850085;
constexpr uint32_t RED = 0xff0000;
constexpr uint32_t TURQUOISE = 0x018384;

constexpr int WINDOW_PIXEL_HEIGHT = 1080;
constexpr int WINDOW_PIXEL_WIDTH = 1920;

constexpr int GRID_HEIGHT = 20; // must fit inside short - see Renderer::draw3DEdges
constexpr int GRID_WIDTH = 20;  // "                                               "

constexpr int FRAME_WIDTH = 20;
constexpr int INFO_PANEL_HEIGHT = 70;

constexpr int CELL_PIXEL_SIZE = 40;
constexpr int CELL_BORDER_WIDTH_3D = 4; // recommend 1/10 of CELL_PIXEL_SIZE
constexpr int CELL_BORDER_WIDTH_2D = 2; // even int
} // namespace config