#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <stdexcept>

#include "engine/engine.hpp"
#include "gui/gui.hpp"

Gui::Gui(int window_width, int window_height, const Engine &engine)
    : layout_{window_width, window_height, engine.row_count(), engine.col_count()}
{
  if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
  {
    SDL_Log("SDL_InitSubSystem failed: %s", SDL_GetError());
    throw std::runtime_error("failed to initialize SDL3");
  }

  if (!SDL_CreateWindowAndRenderer("minesweeper", window_width, window_height, 0, &window_, &renderer_))
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
  SDL_RenderFillRect(renderer_, &layout_.window);

  // dividers
  SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255);
  SDL_RenderFillRects(
      renderer_, layout_.divider_SDL_FRects.data(), static_cast<int>(layout_.divider_SDL_FRects.size()));

  // header
  SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 255);
  SDL_RenderFillRect(renderer_, &layout_.header);

  // board
  SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer_, &layout_.board);

  // cells: inset by a border so the board color shows between them
  constexpr float CELL_BORDER = 2.0F;
  const auto cell = static_cast<float>(layout_.cell_width);
  const int rows = static_cast<int>(layout_.board.h / cell);
  const int cols = static_cast<int>(layout_.board.w / cell);

  for (int r = 0; r < rows; ++r)
  {
    for (int c = 0; c < cols; ++c)
    {
      const SDL_FRect cell_rect{
          .x = layout_.board.x + (static_cast<float>(c) * cell) + CELL_BORDER,
          .y = layout_.board.y + (static_cast<float>(r) * cell) + CELL_BORDER,
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
