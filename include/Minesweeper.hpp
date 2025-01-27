#pragma once

#include <array>
#include <set>
#include <utility>
#include <vector>

class Minesweeper
{
public:
  struct Cell
  {
    bool isMine;
    bool isHidden;
    bool isFlagged;
    unsigned int nAdjacentMines = 0;
  };

  using Minefield = std::vector<Cell>;

  Minesweeper();
  ~Minesweeper() = default;

  const Minefield &getMinefield() { return minefield; }
  const int &getNumMines() { return numMines; }
  const int &getNumFlags() { return numFlags; }
  int getRemainingFlags() { return numMines - numFlags; }
  const int &getSecondsElapsed() { return secondsElapsed; }

  void incrementTimer() { ++secondsElapsed; };

  void handleLeftClick(const int row, const int col);
  void handleRightClick(const int row, const int col);
  void handleMiddleClick(const int row, const int col);

  void reset();

private:
  Minefield minefield;
  int numMines = 0;
  int numFlags = 0;
  int secondsElapsed = 0;
  bool isFirstClick = true;

  // clang-format off
  const std::array<std::pair<int, int>, 8> ADJACENCY_OFFSETS = {{
    { 1, -1}, { 1, 0}, { 1, 1},
    { 0, -1},          { 0, 1},
    {-1, -1}, {-1, 0}, {-1, 1}
  }};
  // clang-format on

  int rowColToIndex(const int row, const int col) const;

  Minefield initMinefield();
  void revealAdjacentCells(const int row, const int col);
  void floodFillEmptyCells(const int row, const int col);
  void floodFillEmptyCellsRecursive(const int row, const int col, std::set<std::pair<int, int>> &visited);
};