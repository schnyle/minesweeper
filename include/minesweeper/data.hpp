#pragma once

#include <cstdint>

#define GRID_HEIGHT 20 // must fit inside short - see Renderer::draw3DEdges
#define GRID_WIDTH 38  // "                                               "

struct Cell
{
  bool isMine;
  bool isHidden;
  bool isFlagged;
  int nAdjacentMines = 0;
};

extern Cell data[GRID_HEIGHT][GRID_WIDTH];

extern uint32_t oneImage[50 * 50];

extern uint32_t flagImage[50 * 50];

std::array<Cell, GRID_HEIGHT * GRID_WIDTH> generateData();