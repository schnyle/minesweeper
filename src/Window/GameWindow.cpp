#include <GameWindow.hpp>
#include <HeaderCompositor.hpp>
#include <SDL2/SDL.h>
#include <config.hpp>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utils.hpp>
#include <vector>

GameWindow::GameWindow() : Window()
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

  window.reset(SDL_CreateWindow(
      "Minesweeper",
      windowX,
      windowY,
      config::GAME_WINDOW_PIXEL_WIDTH,
      config::GAME_WINDOW_PIXEL_HEIGHT,
      SDL_WINDOW_SHOWN));
  SDL_SetWindowPosition(window.get(), windowX, windowY);
  SDL_ShowWindow(window.get());

  if (!window)
  {
    throw std::runtime_error(std::string("error creating game window: ") + SDL_GetError());
  }

  renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

  if (!renderer)
  {
    throw std::runtime_error(std::string("error creating game window renderer: ") + SDL_GetError());
  }

  texture.reset(SDL_CreateTexture(
      renderer.get(),
      SDL_PIXELFORMAT_RGBA8888,
      SDL_TEXTUREACCESS_STREAMING,
      config::GAME_WINDOW_PIXEL_WIDTH,
      config::GAME_WINDOW_PIXEL_HEIGHT));

  if (!texture)
  {
    throw std::runtime_error(std::string("error creating game window texture: ") + SDL_GetError());
  }

  windowID = SDL_GetWindowID(window.get());
};

void GameWindow::update(Minesweeper &gameState)
{
  updateInterface(gameState);
  updateFrameBuffer(gameState);

  void *pixels;
  int pitch;

  SDL_LockTexture(texture.get(), nullptr, &pixels, &pitch);
  std::memcpy(
      pixels,
      frameBuffer.data(),
      config::GAME_WINDOW_PIXEL_WIDTH * config::GAME_WINDOW_PIXEL_HEIGHT * sizeof(uint32_t));

  SDL_UnlockTexture(texture.get());
  SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
  SDL_RenderPresent(renderer.get());
};

void GameWindow::handleEvent(SDL_Event &event, Minesweeper &gameState, bool &isGameLoopRunning)
{
  const int cursorX = event.motion.x;
  const int cursorY = event.motion.y;

  const SDL_Rect resetButtonRect{
      config::RESET_BUTTON_X,
      config::RESET_BUTTON_Y,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT};
  const bool inResetButton = utils::isPointInRect(cursorX, cursorY, resetButtonRect);

  const SDL_Rect configButtonRect{
      config::CONFIG_BUTTON_X,
      config::CONFIG_BUTTON_Y,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT};
  const bool inConfigButton = utils::isPointInRect(cursorX, cursorY, configButtonRect);

  const int row = (cursorY - gameAreaY) / config::CELL_PIXEL_SIZE;
  const int col = (cursorX - gameAreaX) / config::CELL_PIXEL_SIZE;

  const bool inXBounds = cursorX >= gameAreaX && col >= 0 && col < config::GRID_WIDTH;
  const bool inYBounds = cursorY >= gameAreaY && row >= 0 && row < config::GRID_HEIGHT;
  const bool inGameArea = inXBounds && inYBounds;

  switch (event.type)
  {
  case SDL_MOUSEBUTTONDOWN:
  {

    if (inGameArea && !gameState.isGameOver)
    {
      switch (event.button.button)
      {
      case SDL_BUTTON_LEFT:
        gameState.handleLeftClick(row, col);
        break;
      case SDL_BUTTON_MIDDLE:
        gameState.handleMiddleClick(row, col);
        break;
      case SDL_BUTTON_RIGHT:
        gameState.handleRightClick(row, col);
        break;
      }

      gameState.checkForGameWon();
    }

    if (inResetButton && event.button.button == SDL_BUTTON_LEFT)
    {
      gameState.isResetButtonPressed = true;
    }

    if (inConfigButton && event.button.button == SDL_BUTTON_LEFT)
    {
      gameState.isConfigButtonPressed = true;
    }

    break;
  }

  case SDL_MOUSEBUTTONUP:
  {
    if (inResetButton && gameState.isResetButtonPressed)
    {
      gameState.reset();
    }

    if (inConfigButton && gameState.isConfigButtonPressed)
    {
      gameState.showConfigWindow = !gameState.showConfigWindow;
    }

    gameState.isConfigButtonPressed = false;
    gameState.isResetButtonPressed = false;
    break;
  }

  case SDL_KEYDOWN:
    const auto keycode = event.key.keysym.sym;
    if (keycode == SDLK_q || keycode == SDLK_x || keycode == SDLK_ESCAPE)
    {
      isGameLoopRunning = false;
    }
  }
}

void GameWindow::updateInterface(const Minesweeper &gameState)
{
  HeaderCompositor::buffInsertRemainingFlags(
      frameBuffer,
      config::GAME_WINDOW_PIXEL_WIDTH,
      config::REMAINING_FLAGS_X,
      config::REMAINING_FLAGS_Y,
      config::INFO_PANEL_BUTTONS_HEIGHT * 2,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      gameState.getRemainingFlags());

  SpriteFactory::copySprite(
      frameBuffer,
      getResetButtonSprite(gameState),
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::RESET_BUTTON_X,
      config::RESET_BUTTON_Y);

  SpriteFactory::copySprite(
      frameBuffer,
      getConfigButtonSprite(gameState),
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::CONFIG_BUTTON_X,
      config::CONFIG_BUTTON_Y);

  HeaderCompositor::buffInsertRemainingFlags(
      frameBuffer,
      config::GAME_WINDOW_PIXEL_WIDTH,
      config::TIMER_X,
      config::TIMER_Y,
      config::INFO_PANEL_BUTTONS_HEIGHT * 2,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      gameState.getSecondsElapsed());
}

void GameWindow::updateFrameBuffer(const Minesweeper &gameState)
{
  for (int row = 0; row < config::GRID_HEIGHT; ++row)
  {
    for (int col = 0; col < config::GRID_WIDTH; ++col)
    {
      const int cellIndex = row * config::GRID_WIDTH + col;
      const auto &sprite = getCellSprite(gameState, cellIndex);

      // could cache these values so they are only calculated once during construction/init...
      const int x = gameAreaX + col * config::CELL_PIXEL_SIZE;
      const int y = gameAreaY + row * config::CELL_PIXEL_SIZE;
      SpriteFactory::copySprite(frameBuffer, sprite, config::CELL_PIXEL_SIZE, x, y);
    }
  }
};

const std::vector<uint32_t> &GameWindow::getResetButtonSprite(const Minesweeper &gameState) const
{
  if (gameState.getIsResetButtonPressed())
  {
    return sprites->pressedResetButton;
  }

  if (gameState.getIsGameOver())
  {
    return gameState.getIsGameWon() ? sprites->winnerResetButton : sprites->loserResetButton;
  }

  return sprites->raisedResetButton;
}

const std::vector<uint32_t> &GameWindow::getConfigButtonSprite(const Minesweeper &gameState) const
{
  return gameState.getIsConfigButtonPressed() ? sprites->pressedConfigButton : sprites->raisedConfigButton;
}

const std::vector<uint32_t> &GameWindow::getCellSprite(const Minesweeper &gameState, const int cellIndex) const
{
  const auto &[isMine, isHidden, isFlagged, isClicked, nAdjacentMines] = gameState.getMinefield()[cellIndex];

  if (isHidden && !isFlagged)
  {
    return sprites->hidden;
  }
  else if (isHidden && isFlagged && !isMine && gameState.getIsGameOver())
  {
    return sprites->redXMine;
  }
  else if (isHidden && isFlagged)
  {
    return sprites->flag;
  }
  else
  {
    if (isMine)
    {
      return isClicked ? sprites->clickedMine : sprites->mine;
    }
    else
    {
      return sprites->intToSpriteMap[nAdjacentMines];
    }
  }
}