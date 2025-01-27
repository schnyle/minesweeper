#include <Renderer.hpp>
#include <SpriteFactory.hpp>
#include <algorithm>
#include <chrono>
#include <config.hpp>
#include <iostream>
#include <memory>

#include <cstring>

Renderer::Renderer()
{
  initSDL();

  SpriteFactory::buffInsertInterface(frameBuffer.get(), WIDTH, WIDTH * HEIGHT);

  sprites = SpriteFactory::createSprites();
}

Renderer::~Renderer()
{
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Renderer::initSDL()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    std::cout << "error initializing SDL: " << SDL_GetError() << std::endl;
  }

  window = SDL_CreateWindow(
      "Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (!window)
  {
    std::cout << "error creating window: " << SDL_GetError() << std::endl;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer)
  {
    std::cout << "error getting renderer: " << SDL_GetError() << std::endl;
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
  if (!texture)
  {
    std::cout << "error getting texture: " << SDL_GetError() << std::endl;
  }

  frameBuffer = std::make_unique<uint32_t[]>(WIDTH * HEIGHT);
  if (!frameBuffer)
  {
    std::cout << "error allocating frame buffer" << std::endl;
  }
}

void Renderer::renderFrame()
{
  void *pixels;
  int pitch;

  SDL_LockTexture(texture, nullptr, &pixels, &pitch);
  std::memcpy(pixels, frameBuffer.get(), WIDTH * HEIGHT * sizeof(uint32_t));

  SDL_UnlockTexture(texture);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

void Renderer::updateInterface(Minesweeper &game)
{
  // remaining flags
  SpriteFactory::buffInsertRemainingFlags(
      frameBuffer.get(),
      WIDTH,
      config::REMAINING_FLAGS_X,
      config::REMAINING_FLAGS_Y,
      config::INFO_PANEL_BUTTONS_HEIGHT * 2,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      game.getRemainingFlags());

  // reset button
  const auto resetButtonSprite = game.getIsResetButtonPressed() ? sprites->pressedButton : sprites->raisedButton;
  SpriteFactory::copySprite(
      frameBuffer, resetButtonSprite, config::INFO_PANEL_BUTTONS_HEIGHT, config::RESET_BUTTON_X, config::RESET_BUTTON_Y);

  // timer
  SpriteFactory::buffInsertRemainingFlags(
      frameBuffer.get(),
      WIDTH,
      config::TIMER_X,
      config::TIMER_Y,
      config::INFO_PANEL_BUTTONS_HEIGHT * 2,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      game.getSecondsElapsed());
}

void Renderer::updateGameArea(Minesweeper &game)
{
  for (int row = 0; row < config::GRID_HEIGHT; ++row)
  {
    for (int col = 0; col < config::GRID_WIDTH; ++col)
    {
      const int gameAreaX = config::FRAME_WIDTH;
      const int gameAreaY = config::INFO_PANEL_HEIGHT + 2 * config::FRAME_WIDTH;

      const int x = gameAreaX + config::GRID_AREA_X_PAD + col * config::CELL_PIXEL_SIZE;
      const int y = gameAreaY + config::GRID_AREA_Y_PAD + row * config::CELL_PIXEL_SIZE;

      const int index = row * config::GRID_WIDTH + col;
      const auto &[isMine, isHidden, isFlagged, nAdjacentMines] = game.getMinefield()[index];
      uint32_t *sprite;

      if (isHidden)
      {
        sprite = sprites->hidden;

        if (isFlagged)
        {
          sprite = sprites->flag;
        }
      }
      else
      {
        if (isMine)
        {
          sprite = sprites->mine;
        }
        else
        {
          sprite = sprites->intToSpriteMap[nAdjacentMines];
        }
      }

      SpriteFactory::copySprite(frameBuffer, sprite, config::CELL_PIXEL_SIZE, x, y);
    }
  }
}
