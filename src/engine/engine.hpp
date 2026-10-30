#pragma once

#include <format>
#include <iosfwd>
#include <mdspan>
#include <source_location>
#include <stdexcept>
#include <vector>

struct CellPosition
{
  int row, col;

  bool operator==(const CellPosition &) const = default;
};

class Engine
{
public:
  enum class Status : uint8_t
  {
    NotStarted,
    Playing,
    Won,
    Lost,
  };
  friend std::ostream &operator<<(std::ostream &os, Status status);

  enum class GameCellState : uint8_t
  {
    Flagged,
    Hidden,
    Revealed,
  };
  friend std::ostream &operator<<(std::ostream &os, GameCellState state);

  struct GameCell
  {
    bool mine = false;
    GameCellState state = GameCellState::Hidden;
  };

  Engine(int rows, int cols);

  [[nodiscard]] Status status() const { return status_; }
  [[nodiscard]] int row_count() const { return row_count_; }
  [[nodiscard]] int col_count() const { return col_count_; }
  [[nodiscard]] const GameCell &
  game_cell(CellPosition cell_position, std::source_location loc = std::source_location::current()) const;

  void chord(CellPosition cell_position);
  void flag(CellPosition cell_position);
  void reveal(CellPosition cell_position);
  void reset();

private:
  Status status_ = Status::NotStarted;
  const int row_count_;
  const int col_count_;
  std::vector<GameCell> cells_;

  auto &game_cell_impl(
      this auto &self,
      const CellPosition cell_position,
      std::source_location loc = std::source_location::current())
  {
    const auto [row, col] = cell_position;
    if (row < 0 || self.row_count_ <= row || col < 0 || self.col_count_ <= col)
    {
      throw std::out_of_range(
          std::format(
              "{}:{}: (row={}, col={}) out of valid range (row=[0, {}), col=[0, {}))",
              loc.file_name(),
              loc.line(),
              row,
              col,
              self.row_count_,
              self.col_count_));
    }
    return std::mdspan(self.cells_.data(), self.row_count_, self.col_count_)[row, col];
  }
};
