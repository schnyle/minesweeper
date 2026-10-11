#include <SDL3/SDL_rect.h>

#include "gui/layout.hpp"

Layout::Layout(int window_width, int window_height, int rows, int cols)
{
  const SDL_Rect window_px{.x = 0, .y = 0, .w = window_width, .h = window_height};

  const SDL_Rect header_px{
      .x = divider_width,
      .y = divider_width,
      .w = window_width - (2 * divider_width),
      .h = static_cast<int>(window_height * DEFAULT_HEADER_HEIGHT_PCT / 100.0),
  };

  const std::array<SDL_Rect, 5> dividers_px{{
      {.x = 0, .y = 0, .w = window_width, .h = divider_width},
      {.x = 0, .y = header_px.y + header_px.h, .w = window_width, .h = divider_width},
      {.x = 0, .y = window_height - divider_width, .w = window_width, .h = divider_width},
      {.x = 0, .y = 0, .w = divider_width, .h = window_height},
      {.x = window_width - divider_width, .y = 0, .w = divider_width, .h = window_height},
  }};

  // space available for the board: margins above (shared with header), below, and on both sides
  const SDL_Rect board_rect{
      .x = divider_width,
      .y = header_px.h + (2 * divider_width),
      .w = window_width - (2 * divider_width),
      .h = window_height - header_px.h - (3 * divider_width),
  };

  cell_width = std::min(board_rect.w / cols, board_rect.h / rows);

  const int grid_w = cols * cell_width;
  const int grid_h = rows * cell_width;
  const SDL_Rect board_px{
      .x = board_rect.x + ((board_rect.w - grid_w) / 2),
      .y = board_rect.y + ((board_rect.h - grid_h) / 2),
      .w = grid_w,
      .h = grid_h,
  };

  SDL_RectToFRect(&window_px, &window);
  SDL_RectToFRect(&header_px, &header);
  SDL_RectToFRect(&board_px, &board);
  for (std::size_t i = 0; i < divider_SDL_FRects.size(); ++i)
  {
    SDL_RectToFRect(&dividers_px[i], &divider_SDL_FRects[i]);
  }
}

std::optional<CellPosition> Layout::cell_at(const SDL_FPoint &point) const
{
  if (point.x < board.x || point.x >= board.x + board.w || point.y < board.y || point.y >= board.y + board.h)
  {
    return std::nullopt;
  }

  return CellPosition{
      .row = static_cast<int>((point.y - board.y) / cell_width),
      .col = static_cast<int>((point.x - board.x) / cell_width),
  };
}
