#pragma once

#include <config.hpp>

struct Cell
{
  bool isMine;
  bool isHidden;
  bool isFlagged;
  unsigned int nAdjacentMines = 0;
};

std::array<Cell, config::GRID_HEIGHT * config::GRID_WIDTH> generateData();