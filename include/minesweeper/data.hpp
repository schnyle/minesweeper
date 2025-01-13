#pragma once

#include <cstdint>

#define GRID_HEIGHT 9 // must fit inside short - see Renderer::draw3DEdges
#define GRID_WIDTH 9  // "                                               "

struct Cell
{
  bool isMine;
  bool isHidden;
  bool isFlagged;
  int nAdjacentMines;
};

extern Cell data[GRID_HEIGHT][GRID_WIDTH];

extern uint32_t oneImage[50 * 50];

extern uint32_t flagImage[50 * 50];
