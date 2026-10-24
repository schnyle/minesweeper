#pragma once

#include <SDL3/SDL_rect.h>
#include <array>
#include <optional>

#include "engine/engine.hpp"

// all values are pixel counts
// computed in ints so everything lands on whole pixels; rects stored as SDL_FRect since SDL3
// rendering and mouse coordinates are float
struct Layout
{
  static constexpr int DEFAULT_DIVIDER_WIDTH = 20;
  static constexpr double DEFAULT_HEADER_HEIGHT_PCT = 10.0; // percent of window height dedicated to header

  SDL_FRect window;
  SDL_FRect header;
  SDL_FRect board;                             // exactly the grid of cells, centered in the space below the header
  std::array<SDL_FRect, 5> divider_SDL_FRects; // top, middle, bottom, left, right

  int divider_width = DEFAULT_DIVIDER_WIDTH;
  int cell_width;

  Layout(int window_width, int window_height, int rows, int cols);

  [[nodiscard]] std::optional<CellPosition> cell_at(const SDL_FPoint &point) const;
};
