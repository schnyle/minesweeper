#include <algorithm>
#include <array>
#include <config.hpp>
#include <cstdint>
#include <data.hpp>
#include <random>

std::array<Cell, config::GRID_HEIGHT * config::GRID_WIDTH> generateData()
{
  std::random_device rd;
  std::mt19937 rg(rd());
  std::bernoulli_distribution dist(0.2);

  std::array<Cell, config::GRID_HEIGHT * config::GRID_WIDTH> data = {};

  for (size_t i = 0; i < config::GRID_HEIGHT * config::GRID_WIDTH; ++i)
  {
    bool isMine = dist(rg); // random
    bool isHidden = true;
    bool isFlagged = false;

    if (isMine)
    {
      const int row = i / config::GRID_WIDTH;
      const int col = i % config::GRID_WIDTH;

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
