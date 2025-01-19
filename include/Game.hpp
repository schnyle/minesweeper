#pragma once

#include <array>
#include <set>
#include <utility>
#include <vector>

class Game
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

  Game();
  ~Game() = default;

  const Minefield &getMinefield() { return minefield; }

  void handleLeftClick(const int row, const int col);
  void handleRightClick(const int row, const int col);
  void handleMiddleClick(const int row, const int col);

  void reset();

private:
  Minefield minefield;
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