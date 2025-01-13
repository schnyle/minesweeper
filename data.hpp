#pragma once

#include <cstdint>

#define GRID_HEIGHT 9
#define GRID_WIDTH 9

struct Cell
{
  bool isMine;
  bool isRevealed;
  bool isFlagged;
  int nAdjacentMines;
};

extern Cell data[GRID_HEIGHT][GRID_WIDTH];

extern uint32_t onePixel[50 * 50];

extern uint32_t flagPixel[50 * 50];
