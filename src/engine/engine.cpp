#include <algorithm>
#include <ostream>

#include "engine.hpp"

std::ostream &operator<<(std::ostream &os, Engine::Status status)
{
  using enum Engine::Status;

  switch (status)
  {
  case NotStarted:
    return os << "NotStarted";
  case Playing:
    return os << "Playing";
  case Won:
    return os << "Won";
  case Lost:
    return os << "Lost";
  }
  return os;
};

std::ostream &operator<<(std::ostream &os, Engine::GameCellState state)
{
  using enum Engine::GameCellState;

  switch (state)
  {
  case Flagged:
    return os << "Flagged";
  case Hidden:
    return os << "Hidden";
  case Revealed:
    return os << "Revealed";
  }
  return os;
}

Engine::Engine(int rows, int cols)
    : row_count_(rows), col_count_(cols), cells_(static_cast<size_t>(rows) * static_cast<size_t>(cols))
{
}

const Engine::GameCell &Engine::game_cell(const CellPosition cell_position) const
{
  return game_cell_impl(cell_position);
}

void Engine::chord(const CellPosition cell_position)
{
  const auto [row, col] = cell_position;
  const int min_row = std::max(0, row - 1);
  const int max_row = std::min(row_count_ - 1, row + 1);
  const int min_col = std::max(0, col - 1);
  const int max_col = std::min(col_count_ - 1, col + 1);
  for (int i = min_row; i <= max_row; ++i)
  {
    for (int j = min_col; j <= max_col; ++j)
    {
      game_cell_impl({.row = i, .col = j}).state = GameCellState::Revealed;
    }
  }
}

void Engine::flag(const CellPosition cell_position)
{
  GameCell &game_cell = game_cell_impl(cell_position);
  switch (game_cell.state)
  {
  case GameCellState::Hidden:
    game_cell.state = GameCellState::Flagged;
    break;
  case GameCellState::Flagged:
    game_cell.state = GameCellState::Hidden;
    break;
  case GameCellState::Revealed:
    break;
  }
}

void Engine::reveal(const CellPosition cell_position)
{
  GameCell &game_cell = game_cell_impl(cell_position);
  if (game_cell.state == GameCellState::Flagged)
  {
    return;
  }

  game_cell.state = GameCellState::Revealed;
  if (status_ == Status::NotStarted)
  {
    status_ = Status::Playing;
  }
}

void Engine::reset()
{
  status_ = Status::NotStarted;
  std::ranges::fill(cells_, GameCell{});
}
