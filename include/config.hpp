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
constexpr uint32_t BLACK = 0xff000000;
constexpr uint32_t BLUE = 0xff0000ff;

constexpr int GRID_HEIGHT = 4; // must fit inside short - see Renderer::draw3DEdges
constexpr int GRID_WIDTH = 4;  // "                                               "

constexpr int WINDOW_PIXEL_HEIGHT = 950;
constexpr int WINDOW_PIXEL_WIDTH = 1800;

constexpr int FRAME_WIDTH = 20;
constexpr int INFO_PANEL_HEIGHT = 70;

// sprites
constexpr int CELL_PIXEL_SIZE = 50;
constexpr int CELL_BORDER_WIDTH_3D = 5;    // recommend 1/10 of CELL_PIXEL_SIZE
constexpr int CELL_BORDER_WIDTH_2D = 2;    // even int
constexpr size_t IMAGE_SIZE = 50 * 50 * 4; // images in assets/

constexpr double FLAG_BOTTOM_BASE_HEIGHT_RATIO = 0.1;
constexpr double FLAG_BOTTOM_BASE_WIDTH_RATIO = 0.5;
constexpr double FLAG_TOP_BASE_RECT_HEIGHT_RATIO = 0.05;
constexpr double FLAG_TOP_BASE_RECT_WIDTH_RATIO = 0.33;
constexpr double FLAG_POLE_WIDTH_RATIO = 0.05;
constexpr double FLAG_SIZE_RATIO = 0.3;
constexpr double FLAG_SLOPE = 0.66;
constexpr double FLAG_TOTAL_HEIGHT_RATIO = 0.55;
} // namespace config