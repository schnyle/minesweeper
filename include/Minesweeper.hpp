#pragma once

#include <SDL2/SDL.h>
#include <array>
#include <set>
#include <utility>
#include <vector>

class Minesweeper
{
private:
  struct Cell
  {
    bool isMine;
    bool isHidden;
    bool isFlagged;
    bool isClicked = false;
    unsigned int nAdjacentMines = 0;
  };

public:
  using Minefield = std::vector<Cell>;

  Minesweeper();
  ~Minesweeper() = default;

  const Minefield &getMinefield() const { return minefield; }
  int getNumMines() const { return numMines; }
  int getNumFlags() const { return numFlags; }
  int getRemainingFlags() const { return numMines - numFlags; }
  int getSecondsElapsed() const { return secondsElapsed; }
  bool getIsGameOver() const { return isGameOver; }
  bool getIsGameWon() const { return isGameWon; }
  bool getIsResetButtonPressed() const { return isResetButtonPressed; }
  bool getIsConfigButtonPressed() const { return isConfigButtonPressed; }
  bool getShowConfigButton() const { return showConfigWindow; };

  void setIsResetButtonPressed(const bool newVal) { isResetButtonPressed = newVal; }
  void setIsConfigButtonPressed(const bool newVal) { isConfigButtonPressed = newVal; }
  void setShowConfigWindow(const bool newVal) { showConfigWindow = newVal; }

  void handleLeftClick(const int row, const int col);
  void handleRightClick(const int row, const int col);
  void handleMiddleClick(const int row, const int col);
  void incrementTimer() { ++secondsElapsed; };
  void checkForGameWon();
  void reset();

private:
  Minefield minefield;
  int numMines = 0;
  int numFlags = 0;
  int secondsElapsed = 0;
  bool isGameOver = false;
  bool isGameWon = false;
  bool isFirstClick = true;
  bool isResetButtonPressed = false;
  bool isConfigButtonPressed = false;
  bool showConfigWindow = false;

  // clang-format off
  const std::array<std::pair<int, int>, 8> ADJACENCY_OFFSETS = {{
    { 1, -1}, { 1, 0}, { 1, 1},
    { 0, -1},          { 0, 1},
    {-1, -1}, {-1, 0}, {-1, 1}
  }};
  // clang-format on

  Minefield initMinefield();
  int rowColToIndex(const int row, const int col) const;
  void revealAdjacentCells(const int row, const int col);
  void floodFillEmptyCells(const int row, const int col);
  void floodFillEmptyCellsRecursive(const int row, const int col, std::set<std::pair<int, int>> &visited);
};