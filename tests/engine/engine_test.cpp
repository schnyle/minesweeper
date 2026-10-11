#include "engine/engine.hpp"

#include <gtest/gtest.h>
#include <ranges>

TEST(EngineTest, InitCorrectBoardDimensions)
{
  const int rows = 10;
  const int cols = 5;

  Engine engine(rows, cols);

  EXPECT_EQ(rows, engine.row_count());
  EXPECT_EQ(cols, engine.col_count());
}

TEST(EngineTest, InitAllCellsHidden)
{
  const int rows = 10;
  const int cols = 5;

  Engine engine(rows, cols);

  for (auto [r, c] : std::views::cartesian_product(std::views::iota(0, rows), std::views::iota(0, cols)))
  {
    EXPECT_EQ(engine.cell(r, c).state, Engine::CellState::Hidden) << "at (" << r << ", " << c << ")";
  }
}
