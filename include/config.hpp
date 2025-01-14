#pragma once

namespace config
{
constexpr int GRID_HEIGHT = 20; // must fit inside short - see Renderer::draw3DEdges
constexpr int GRID_WIDTH = 38;  // "                                               "

constexpr int CELL_PIXEL_SIZE = 50;
} // namespace config