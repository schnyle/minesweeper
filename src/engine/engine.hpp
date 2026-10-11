#pragma once

#include <format>
#include <mdspan>
#include <stdexcept>
#include <vector>

class Engine
{
public:
  enum class CellState : uint8_t
  {
    Hidden,
    Revealed,
    Flagged,
  };

  struct Cell
  {
    bool mine = false;
    CellState state = CellState::Hidden;
  };

  Engine(int rows, int cols);

  [[nodiscard]] int row_count() const { return row_count_; }
  [[nodiscard]] int col_count() const { return col_count_; }

  [[nodiscard]] const Cell &cell(int row, int col) const;

  void reveal(int row, int col);
  void flag(int row, int col);
  void chord(int row, int col);

private:
  const int row_count_;
  const int col_count_;
  std::vector<Cell> cells_;

  // TODO: understand this more
  auto grid(this auto &self) { return std::mdspan(self.cells_.data(), self.row_count_, self.col_count_); };

  auto &cell_impl(this auto &self, int row, int col)
  {
    if (row < 0 || self.row_count_ <= row || col < 0 || self.col_count_ <= col)
    {
      throw std::out_of_range(std::format("Engine::cell(): got out of range (row, col): ({}, {})", row, col));
    }
    return self.grid()[row, col];
  }
};
