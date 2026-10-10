#include "engine/engine.hpp"

#include <gtest/gtest.h>

TEST(EngineTest, Initialization)
{
  const int rows = 10;
  const int cols = 5;

  Engine engine(rows, cols);

  EXPECT_EQ(rows, engine.row_count());
  EXPECT_EQ(cols, engine.col_count());
}
