#include <GameWindow.hpp>
#include <HeaderCompositor.hpp>
#include <SDL2/SDL.h>
#include <config.hpp>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

GameWindow::GameWindow()
{
  frameBuffer.resize(config::GAME_WINDOW_PIXEL_WIDTH * config::GAME_WINDOW_PIXEL_HEIGHT);

  HeaderCompositor::buffInsertHeader(
      frameBuffer, config::GAME_WINDOW_PIXEL_WIDTH, config::GAME_WINDOW_PIXEL_WIDTH * config::GAME_WINDOW_PIXEL_HEIGHT);
}

void GameWindow::init()
{
  SDL_Rect bounds;
  SDL_GetDisplayBounds(0, &bounds);

  const int leftPad = (config::DISPLAY_PIXEL_WIDTH - config::GAME_WINDOW_PIXEL_WIDTH) / 2;
  const int topPad = (config::DISPLAY_PIXEL_HEIGHT - config::GAME_WINDOW_PIXEL_HEIGHT) / 2;

  const int windowX = bounds.x + leftPad;
  const int windowY = bounds.y + topPad;

  window = SDL_CreateWindow(
      "Minesweeper",
      windowX,
      windowY,
      config::GAME_WINDOW_PIXEL_WIDTH,
      config::GAME_WINDOW_PIXEL_HEIGHT,
      SDL_WINDOW_SHOWN);
  SDL_SetWindowPosition(window, windowX, windowY);
  SDL_ShowWindow(window);

  if (window == nullptr)
  {
    std::cerr << "error creating game window: " << SDL_GetError() << std::endl;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (renderer == nullptr)
  {
    std::cerr << "error creating game window renderer: " << SDL_GetError() << std::endl;
  }

  texture = SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_RGBA8888,
      SDL_TEXTUREACCESS_STREAMING,
      config::GAME_WINDOW_PIXEL_WIDTH,
      config::GAME_WINDOW_PIXEL_HEIGHT);

  if (texture == nullptr)
  {
    std::cerr << "error creating game window texture" << SDL_GetError() << std::endl;
  }

  windowID = SDL_GetWindowID(window);
};

void GameWindow::update(Minesweeper &minesweeper)
{
  updateInterface(minesweeper);
  updateGameArea(minesweeper);

  void *pixels;
  int pitch;

  SDL_LockTexture(texture, nullptr, &pixels, &pitch);
  std::memcpy(
      pixels,
      frameBuffer.data(),
      config::GAME_WINDOW_PIXEL_WIDTH * config::GAME_WINDOW_PIXEL_HEIGHT * sizeof(uint32_t));

  SDL_UnlockTexture(texture);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
};

void GameWindow::updateInterface(Minesweeper &minesweeper)
{
  // remaining flags
  HeaderCompositor::buffInsertRemainingFlags(
      frameBuffer,
      config::GAME_WINDOW_PIXEL_WIDTH,
      config::REMAINING_FLAGS_X,
      config::REMAINING_FLAGS_Y,
      config::INFO_PANEL_BUTTONS_HEIGHT * 2,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      minesweeper.getRemainingFlags());

  // reset button
  auto resetButtonSprite = sprites->raisedResetButton;

  if (minesweeper.getIsGameOver())
  {
    resetButtonSprite = minesweeper.getIsGameWon() ? sprites->winnerResetButton : sprites->loserResetButton;
  }

  if (minesweeper.getIsResetButtonPressed())
  {
    resetButtonSprite = sprites->pressedResetButton;
  }

  SpriteFactory::copySprite(
      frameBuffer, resetButtonSprite, config::INFO_PANEL_BUTTONS_HEIGHT, config::RESET_BUTTON_X, config::RESET_BUTTON_Y);

  // config button
  const auto configButtonSprite = minesweeper.getIsConfigButtonPressed() ? sprites->pressedButton
                                                                         : sprites->raisedButton;
  SpriteFactory::copySprite(
      frameBuffer,
      configButtonSprite,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::CONFIG_BUTTON_X,
      config::CONFIG_BUTTON_Y);

  // timer
  HeaderCompositor::buffInsertRemainingFlags(
      frameBuffer,
      config::GAME_WINDOW_PIXEL_WIDTH,
      config::TIMER_X,
      config::TIMER_Y,
      config::INFO_PANEL_BUTTONS_HEIGHT * 2,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      minesweeper.getSecondsElapsed());
}

void GameWindow::updateGameArea(Minesweeper &game)
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
      const auto &[isMine, isHidden, isFlagged, isClicked, nAdjacentMines] = game.getMinefield()[index];
      std::vector<uint32_t> sprite;

      if (isHidden && !isFlagged)
      {
        sprite = sprites->hidden;
      }
      else if (isHidden && isFlagged && !isMine && game.getIsGameOver())
      {
        sprite = sprites->redXMine;
      }
      else if (isHidden && isFlagged)
      {
        sprite = sprites->flag;
      }
      else
      {
        if (isMine)
        {
          sprite = isClicked ? sprites->clickedMine : sprites->mine;
        }
        else
        {
          sprite = sprites->intToSpriteMap[nAdjacentMines];
        }
      }

      SpriteFactory::copySprite(frameBuffer, sprite, config::CELL_PIXEL_SIZE, x, y);
    }
  }
};