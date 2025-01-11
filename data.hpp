#pragma once

#define GRID_HEIGHT 9
#define GRID_WIDTH 9

struct Cell
{
  bool isMine;
  bool isRevealed;
  bool isFlagged;
  int nAdjacentMines;
};

Cell data[GRID_HEIGHT][GRID_WIDTH]{
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    },
    {
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
        {false, false, false, 1},
    }};