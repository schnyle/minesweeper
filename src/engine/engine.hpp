#pragma once

class Engine
{
public:
  Engine(int rows, int cols);

  int row_count() const { return row_count_; }
  int col_count() const { return col_count_; }

private:
  const int row_count_;
  const int col_count_;
};
