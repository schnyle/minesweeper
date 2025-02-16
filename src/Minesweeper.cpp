#include <Minesweeper.hpp>
#include <SDL2/SDL.h>
#include <config.hpp>
#include <iostream>
#include <random>
#include <set>
#include <utility>
#include <vector>

Minesweeper::Minesweeper() { minefield = initMinefield(); }

void Minesweeper::handleLeftClick(const int row, const int col)
{
  const auto index = rowColToIndex(row, col);
  auto &cell = minefield[index];

  if (isFirstClick)
  {
    isFirstClick = false;
    while (cell.nAdjacentMines != 0 || cell.isMine)
    {
      minefield = initMinefield();
    }
  }

  if (cell.isFlagged || !cell.isHidden)
  {
    return;
  }

  cell.isHidden = false;

  if (cell.isMine) // will be gameover here
  {
    ++numFlags;
    for (auto &xCell : minefield)
    {
      if (xCell.isMine)
      {
        xCell.isHidden = false;
      }
    }
    return;
  }

  if (cell.nAdjacentMines == 0)
  {
    floodFillEmptyCells(row, col);
  }
};

void Minesweeper::handleRightClick(const int row, const int col)
{
  auto &cell = minefield[rowColToIndex(row, col)];

  if (!cell.isHidden)
  {
    return;
  }

  if (!cell.isFlagged && numFlags == numMines)
  {
    return;
  }

  cell.isFlagged = !cell.isFlagged;

  numFlags += cell.isFlagged ? 1 : -1;
};

void Minesweeper::handleMiddleClick(const int row, const int col)
{
  const auto index = rowColToIndex(row, col);

  if (minefield[index].isHidden)
  {
    return;
  }

  revealAdjacentCells(row, col);
};

void Minesweeper::reset()
{
  minefield = initMinefield();
  isFirstClick = true;
}

std::vector<Minesweeper::Cell> Minesweeper::initMinefield()
{
  std::random_device rd;
  std::mt19937 rg(rd());
  std::bernoulli_distribution dist(0.2);

  std::vector<Cell> data(config::GRID_HEIGHT * config::GRID_WIDTH);
  numMines = 0;
  numFlags = 0;
  secondsElapsed = 0;

  for (int i = 0; i < config::GRID_HEIGHT * config::GRID_WIDTH; ++i)
  {
    bool isMine = dist(rg); // random
    bool isHidden = true;
    bool isFlagged = false;

    if (isMine)
    {
      const int row = i / config::GRID_WIDTH;
      const int col = i % config::GRID_WIDTH;

      ++numMines;

      // right
      if (col != config::GRID_WIDTH - 1)
      {
        ++data[i + 1].nAdjacentMines;
      }

      // left
      if (col != 0)
      {
        ++data[i - 1].nAdjacentMines;
      }

      // top
      if (row != 0)
      {
        ++data[i - config::GRID_WIDTH].nAdjacentMines;
      }

      // bot
      if (row != config::GRID_HEIGHT - 1)
      {
        ++data[i + config::GRID_WIDTH].nAdjacentMines;
      }

      // top-left
      if (row != 0 && col != 0)
      {
        ++data[i - config::GRID_WIDTH - 1].nAdjacentMines;
      }

      // top-right
      if (row != 0 && col != config::GRID_WIDTH - 1)
      {
        ++data[i - config::GRID_WIDTH + 1].nAdjacentMines;
      }

      // bot-left
      if (row != config::GRID_HEIGHT - 1 && col != 0)
      {
        ++data[i + config::GRID_WIDTH - 1].nAdjacentMines;
      }

      // bot-right
      if (row != config::GRID_HEIGHT - 1 && col != config::GRID_WIDTH - 1)
      {
        ++data[i + config::GRID_WIDTH + 1].nAdjacentMines;
      }
    }

    data[i] = {isMine, isHidden, isFlagged, data[i].nAdjacentMines};
  }

  return data;
}

int Minesweeper::rowColToIndex(const int row, const int col) const { return row * config::GRID_WIDTH + col; }

void Minesweeper::revealAdjacentCells(const int row, const int col)
{
  unsigned int nFlags = 0;
  std::set<std::pair<int, int>> hidden;

  for (const auto &[dRow, dCol] : ADJACENCY_OFFSETS)
  {
    const int currentRow = row + dRow;
    const int currentCol = col + dCol;
    if (currentRow < 0 || currentCol < 0 || currentRow >= config::GRID_HEIGHT || currentCol >= config::GRID_WIDTH)
    {
      continue;
    }

    const auto currentIndex = rowColToIndex(currentRow, currentCol);
    if (minefield[currentIndex].isFlagged)
    {
      ++nFlags;
    }
    else if (minefield[currentIndex].isHidden)
    {
      hidden.insert({currentRow, currentCol});
    }
  }

  if (nFlags == minefield[rowColToIndex(row, col)].nAdjacentMines)
  {
    for (const auto &[currentRow, currentCol] : hidden)
    {
      const auto currentIndex = rowColToIndex(currentRow, currentCol);
      minefield[currentIndex].isHidden = false;
      if (minefield[currentIndex].nAdjacentMines == 0)
      {
        floodFillEmptyCells(currentRow, currentCol);
      }
    }
  }
}

void Minesweeper::floodFillEmptyCells(int row, int col)
{
  std::set<std::pair<int, int>> visited;
  floodFillEmptyCellsRecursive(row, col, visited);
}

void Minesweeper::floodFillEmptyCellsRecursive(int row, int col, std::set<std::pair<int, int>> &visited)
{
  visited.insert({row, col});

  for (const auto &[dRow, dCol] : ADJACENCY_OFFSETS)
  {
    const int newRow = row + dRow;
    const int newCol = col + dCol;
    if (newRow < 0 || newCol < 0 || newRow >= config::GRID_HEIGHT || newCol >= config::GRID_WIDTH)
    {
      continue;
    }

    const std::pair<int, int> p{newRow, newCol};
    if (visited.find(p) != visited.end())
    {
      continue;
    }
    else
    {
      visited.insert(p);
    }

    const int index = p.first * config::GRID_WIDTH + p.second;
    if (!minefield[index].isMine)
    {
      minefield[index].isHidden = false;
      if (minefield[index].nAdjacentMines == 0)
      {
        floodFillEmptyCellsRecursive(newRow, newCol, visited);
      }
    }
  }
}

bool Minesweeper::updateGameState(SDL_Event &event)
{
  const int cursorX = event.motion.x;
  const int cursorY = event.motion.y;

  const bool inResetButton = cursorX >= config::RESET_BUTTON_X &&
                             cursorX < config::RESET_BUTTON_X + config::INFO_PANEL_BUTTONS_HEIGHT &&
                             cursorY >= config::RESET_BUTTON_Y &&
                             cursorY < config::RESET_BUTTON_Y + config::INFO_PANEL_BUTTONS_HEIGHT;

  const bool inConfigButton = cursorX >= config::CONFIG_BUTTON_X &&
                              cursorX < config::CONFIG_BUTTON_X + config::INFO_PANEL_BUTTONS_HEIGHT &&
                              cursorY >= config::CONFIG_BUTTON_Y &&
                              cursorY < config::CONFIG_BUTTON_Y + config::INFO_PANEL_BUTTONS_HEIGHT;

  const int gameAreaX = config::FRAME_WIDTH + config::GRID_AREA_X_PAD;
  const int gameAreaY = config::INFO_PANEL_HEIGHT + 2 * config::FRAME_WIDTH + config::GRID_AREA_Y_PAD;

  const int row = (cursorY - gameAreaY) / config::CELL_PIXEL_SIZE;
  const int col = (cursorX - gameAreaX) / config::CELL_PIXEL_SIZE;

  const bool inXBounds = cursorX >= gameAreaX && col >= 0 && col < config::GRID_WIDTH;
  const bool inYBounds = cursorY >= gameAreaY && row >= 0 && row < config::GRID_HEIGHT;
  const bool inGameArea = inXBounds && inYBounds;

  switch (event.type)
  {
  case SDL_MOUSEBUTTONDOWN:
  {

    if (inGameArea)
    {
      switch (event.button.button)
      {
      case SDL_BUTTON_LEFT:
        handleLeftClick(row, col);
        break;
      case SDL_BUTTON_MIDDLE:
        handleMiddleClick(row, col);
        break;
      case SDL_BUTTON_RIGHT:
        handleRightClick(row, col);
        break;
      }
    }

    if (inResetButton && event.button.button == SDL_BUTTON_LEFT)
    {
      isResetButtonPressed = true;
    }

    if (inConfigButton && event.button.button == SDL_BUTTON_LEFT)
    {
      isConfigButtonPressed = true;
    }

    break;
  }

  case SDL_MOUSEBUTTONUP:
  {
    if (inResetButton && isResetButtonPressed)
    {
      reset();
    }

    if (inConfigButton && isConfigButtonPressed)
    {
      showConfigWindow = !showConfigWindow;
    }

    isConfigButtonPressed = false;
    isResetButtonPressed = false;
    break;
  }

  case SDL_KEYDOWN:
    const auto keycode = event.key.keysym.sym;
    if (keycode == SDLK_q || keycode == SDLK_x || keycode == SDLK_ESCAPE)
    {
      return false;
    }
  }

  return true;
}