#pragma once

#include <SDL3/SDL_rect.h>
#include <optional>
#include <variant>

#include "engine/engine.hpp"

struct ResetButton
{
  bool operator==(const ResetButton &) const = default;
};

using HitTarget = std::variant<CellPosition, ResetButton>;

struct WindowSizePixels
{
  int width;
  int height;
};

struct GridSize
{
  int rows;
  int cols;
};

struct LayoutStyle
{
  int divider_width;
  double header_height_pct;
  double reset_button_cell_span;
};

struct DividerRects
{
  SDL_Rect top;
  SDL_Rect middle; // between header and body
  SDL_Rect bottom;
  SDL_Rect left;
  SDL_Rect right;
};

struct Layout
{
  Layout(WindowSizePixels window_size, GridSize grid_size, LayoutStyle style);

  int cell_width;
  SDL_Rect window_rect;
  SDL_Rect header_rect;
  SDL_Rect reset_button_rect;
  SDL_Rect body_rect;
  SDL_Rect grid_rect;
  DividerRects divider_rects;

  [[nodiscard]] std::optional<CellPosition> cell_at(const SDL_Point &point) const;
  [[nodiscard]] std::optional<HitTarget> hit_test(const SDL_Point &point) const;
};
