#include <SDL3/SDL_rect.h>
#include <algorithm>
#include <format>
#include <source_location>
#include <stdexcept>

#include "engine/engine.hpp"
#include "gui/layout.hpp"

namespace
{
SDL_Rect _centered_in(const SDL_Rect &outer, int w, int h)
{
  return {.x = outer.x + ((outer.w - w) / 2), .y = outer.y + ((outer.h - h) / 2), .w = w, .h = h};
}

void _validate_grid_size(GridSize grid_size)
{
  if (grid_size.rows <= 0 || grid_size.cols <= 0)
  {
    throw std::invalid_argument(
        std::format(
            "{}: grid rows & cols must be positive (got rows={}, cols={})",
            std::source_location::current().function_name(),
            grid_size.rows,
            grid_size.cols));
  }
}

void _validate_window_size(WindowSizePixels window_size)
{
  if (window_size.width <= 0 || window_size.height <= 0)
  {
    throw std::invalid_argument(
        std::format(
            "{}: window size must be positive (got width={}, height={})",
            std::source_location::current().function_name(),
            window_size.width,
            window_size.height));
  }
}

SDL_Rect _window_rect(WindowSizePixels window_size)
{
  return SDL_Rect{
      .x = 0,
      .y = 0,
      .w = window_size.width,
      .h = window_size.height,
  };
}

SDL_Rect _header_rect(WindowSizePixels window_size, LayoutStyle style)
{
  return SDL_Rect{
      .x = style.divider_width,
      .y = style.divider_width,
      .w = window_size.width - (2 * style.divider_width),
      .h = static_cast<int>(window_size.height * style.header_height_pct / 100.0),
  };
}

SDL_Rect _reset_button(SDL_Rect header_rect, int cell_width, LayoutStyle style)
{
  const int reset_size = std::min(static_cast<int>(cell_width * style.reset_button_cell_span), header_rect.h);
  return _centered_in(header_rect, reset_size, reset_size);
}

DividerRects _divider_rects(WindowSizePixels window_size, int header_bottom, LayoutStyle style)
{
  const int d = style.divider_width;
  return DividerRects{
      .top = {.x = 0, .y = 0, .w = window_size.width, .h = d},
      .middle = {.x = 0, .y = header_bottom, .w = window_size.width, .h = d},
      .bottom = {.x = 0, .y = window_size.height - d, .w = window_size.width, .h = d},
      .left = {.x = 0, .y = 0, .w = d, .h = window_size.height},
      .right = {.x = window_size.width - d, .y = 0, .w = d, .h = window_size.height},
  };
}

SDL_Rect _body_rect(SDL_Rect window_rect, SDL_Rect header_rect, LayoutStyle style)
{
  return SDL_Rect{
      .x = style.divider_width,
      .y = header_rect.y + header_rect.h + style.divider_width,
      .w = header_rect.w,
      .h = window_rect.h - header_rect.h - (3 * style.divider_width),
  };
}
} // namespace

Layout::Layout(WindowSizePixels window_size, GridSize grid_size, LayoutStyle style)
{
  _validate_grid_size(grid_size);
  _validate_window_size(window_size);

  const auto [rows, cols] = grid_size;

  window_rect = _window_rect(window_size);
  header_rect = _header_rect(window_size, style);
  const int header_bottom = header_rect.y + header_rect.h;

  divider_rects = _divider_rects(window_size, header_bottom, style);
  body_rect = _body_rect(window_rect, header_rect, style);
  cell_width = std::min(body_rect.w / cols, body_rect.h / rows);
  grid_rect = _centered_in(body_rect, cols * cell_width, rows * cell_width);
  reset_button_rect = _reset_button(header_rect, cell_width, style);
}

std::optional<CellPosition> Layout::cell_at(const SDL_Point &point) const
{
  if (!SDL_PointInRect(&point, &grid_rect))
  {
    return std::nullopt;
  }

  return CellPosition{
      .row = (point.y - grid_rect.y) / cell_width,
      .col = (point.x - grid_rect.x) / cell_width,
  };
}

std::optional<HitTarget> Layout::hit_test(const SDL_Point &point) const
{
  if (SDL_PointInRect(&point, &reset_button_rect))
  {
    return ResetButton{};
  }

  if (auto cell = cell_at(point))
  {
    return *cell;
  }

  return std::nullopt;
}
