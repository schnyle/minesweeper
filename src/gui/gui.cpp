#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <stdexcept>

#include "engine/engine.hpp"
#include "gui/gui.hpp"
#include "gui/layout.hpp"

namespace
{
// layout is in whole pixels; SDL3 rendering takes floats
void _fill_rect(SDL_Renderer *renderer, const SDL_Rect &rect)
{
  SDL_FRect frect{};
  SDL_RectToFRect(&rect, &frect);
  SDL_RenderFillRect(renderer, &frect);
}
} // namespace

Gui::Gui(WindowSizePixels window_size, LayoutStyle layout_style, const Engine &engine)
    : layout_{window_size, {.rows = engine.row_count(), .cols = engine.col_count()}, layout_style}
{
  if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
  {
    SDL_Log("SDL_InitSubSystem failed: %s", SDL_GetError());
    throw std::runtime_error("failed to initialize SDL3");
  }

  if (!SDL_CreateWindowAndRenderer("minesweeper", window_size.width, window_size.height, 0, &window_, &renderer_))
  {
    SDL_Log("window/renderer creation failed: %s", SDL_GetError());
    SDL_Quit();
    throw std::runtime_error("failed to create SDL3 window/renderer");
  }
}

Gui::~Gui()
{
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Gui::render(const Engine &engine)
{
  // background
  SDL_SetRenderDrawColor(renderer_, 40, 40, 40, 255);
  _fill_rect(renderer_, layout_.window_rect);

  // dividers
  SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255);
  _fill_rect(renderer_, layout_.divider_rects.top);
  _fill_rect(renderer_, layout_.divider_rects.middle);
  _fill_rect(renderer_, layout_.divider_rects.bottom);
  _fill_rect(renderer_, layout_.divider_rects.left);
  _fill_rect(renderer_, layout_.divider_rects.right);

  // header
  SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 255);
  _fill_rect(renderer_, layout_.header_rect);

  // board
  SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
  _fill_rect(renderer_, layout_.grid_rect);

  // cells: inset by a border so the board color shows between them
  constexpr float CELL_BORDER = 2.0F;
  const auto cell = static_cast<float>(layout_.cell_width);
  const int rows = engine.row_count();
  const int cols = engine.col_count();

  for (int r = 0; r < rows; ++r)
  {
    for (int c = 0; c < cols; ++c)
    {
      const SDL_FRect cell_rect{
          .x = static_cast<float>(layout_.grid_rect.x) + (static_cast<float>(c) * cell) + CELL_BORDER,
          .y = static_cast<float>(layout_.grid_rect.y) + (static_cast<float>(r) * cell) + CELL_BORDER,
          .w = cell - (2 * CELL_BORDER),
          .h = cell - (2 * CELL_BORDER),
      };
      SDL_SetRenderDrawColor(renderer_, 160, 160, 160, 255);
      SDL_RenderFillRect(renderer_, &cell_rect);

      SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
      switch (engine.game_cell({.row = r, .col = c}).state)
      {
      case Engine::GameCellState::Flagged:
        SDL_RenderDebugText(renderer_, cell_rect.x, cell_rect.y, "F");
        break;
      case Engine::GameCellState::Hidden:
        SDL_RenderDebugText(renderer_, cell_rect.x, cell_rect.y, "H");
        break;
      case Engine::GameCellState::Revealed:
        SDL_RenderDebugText(renderer_, cell_rect.x, cell_rect.y, "R");
        break;
      }
    }
  }

  SDL_RenderPresent(renderer_);
}
