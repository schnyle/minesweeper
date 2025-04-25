#include <GameWindow.hpp>
#include <HeaderArtist.hpp>
#include <MinefieldArtist.hpp>
#include <SDL2/SDL.h>
#include <Sprites.hpp>
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
  frameBuffer.resize(config::getSettings().getGameWindowWidth() * config::getSettings().getGameWindowHeight());

  HeaderArtist::drawHeader(
      frameBuffer,
      config::getSettings().getGameWindowWidth(),
      config::getSettings().getGameWindowWidth() * config::getSettings().getGameWindowHeight());
}

void GameWindow::init()
{
  SDL_Rect bounds;
  SDL_GetDisplayBounds(0, &bounds);

  const int leftPad = (config::getSettings().getDisplayWidth() - config::getSettings().getGameWindowWidth()) / 2;
  const int topPad = (config::getSettings().getDisplayHeight() - config::getSettings().getGameWindowHeight()) / 2;

  const int windowX = bounds.x + leftPad;
  const int windowY = bounds.y + topPad;

  window.reset(SDL_CreateWindow(
      "Minesweeper",
      windowX,
      windowY,
      config::getSettings().getGameWindowWidth(),
      config::getSettings().getGameWindowHeight(),
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
      config::getSettings().getGameWindowWidth(),
      config::getSettings().getGameWindowHeight()));

  if (!texture)
  {
    throw std::runtime_error(std::string("error creating game window texture: ") + SDL_GetError());
  }

  windowID = SDL_GetWindowID(window.get());
};

void GameWindow::update(Minesweeper &gameState)
{
  HeaderArtist::updateHeader(frameBuffer, config::getSettings().getGameWindowWidth(), gameState);
  MinefieldArtist::updateMinefield(frameBuffer, config::getSettings().getCellPixelSize(), gameState);

  void *pixels;
  int pitch;

  SDL_LockTexture(texture.get(), nullptr, &pixels, &pitch);
  std::memcpy(
      pixels,
      frameBuffer.data(),
      config::getSettings().getGameWindowWidth() * config::getSettings().getGameWindowHeight() * sizeof(uint32_t));

  SDL_UnlockTexture(texture.get());
  SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
  SDL_RenderPresent(renderer.get());
};

void GameWindow::handleEvent(SDL_Event &event, Minesweeper &gameState, bool &isGameLoopRunning) const
{
  const int cursorX = event.motion.x;
  const int cursorY = event.motion.y;

  const SDL_Rect resetButtonRect{
      config::getSettings().getResetButtonX(),
      config::getSettings().getResetButtonY(),
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT};
  const bool inResetButton = utils::isPointInRect(cursorX, cursorY, resetButtonRect);

  const SDL_Rect configButtonRect{
      config::getSettings().getConfigButtonX(),
      config::getSettings().getConfigButtonY(),
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT};
  const bool inConfigButton = utils::isPointInRect(cursorX, cursorY, configButtonRect);

  const int row = (cursorY - gameAreaY) / config::getSettings().getCellPixelSize();
  const int col = (cursorX - gameAreaX) / config::getSettings().getCellPixelSize();

  const bool inXBounds = cursorX >= gameAreaX && col >= 0 && col < config::getSettings().getGridWidth();
  const bool inYBounds = cursorY >= gameAreaY && row >= 0 && row < config::getSettings().getGridHeight();
  const bool inGameArea = inXBounds && inYBounds;

  switch (event.type)
  {
  case SDL_MOUSEBUTTONDOWN:
  {

    if (inGameArea && !gameState.getIsGameOver())
    {
      switch (event.button.button)
      {
      case SDL_BUTTON_LEFT:
        if (SDL_GetModState() & KMOD_CTRL)
        {
          gameState.handleMiddleClick(row, col);
        }
        else
        {
          gameState.handleLeftClick(row, col);
        }
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
      gameState.setIsResetButtonPressed(true);
    }

    if (inConfigButton && event.button.button == SDL_BUTTON_LEFT)
    {
      gameState.setIsConfigButtonPressed(true);
    }

    break;
  }

  case SDL_MOUSEBUTTONUP:
  {
    if (inResetButton && gameState.getIsResetButtonPressed())
    {
      gameState.reset();
    }

    if (inConfigButton && gameState.getIsConfigButtonPressed())
    {
      gameState.setShowConfigWindow(!gameState.getShowConfigButton());
    }

    gameState.setIsConfigButtonPressed(false);
    gameState.setIsResetButtonPressed(false);
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