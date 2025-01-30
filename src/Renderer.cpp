#include <Minesweeper.hpp>
#include <Renderer.hpp>
#include <SpriteFactory.hpp>
#include <algorithm>
#include <chrono>
#include <config.hpp>
#include <cstring>
#include <iostream>
#include <memory>

#include "font.h"
#include <SDL.h>
#include <SDL_ttf.h>

Renderer::Renderer()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    std::cout << "error initializing SDL: " << SDL_GetError() << std::endl;
  }

  if (TTF_Init() < 0)
  {
    std::cout << "error initializing SDL TTF: " << TTF_GetError() << std::endl;
  }

  SDL_RWops *rw = SDL_RWFromMem(assets_UbuntuMono_B_ttf, assets_UbuntuMono_B_ttf_len);
  font24 = TTF_OpenFontRW(rw, 0, 24);
  font48 = TTF_OpenFontRW(rw, 0, 48);
  if (font24 == nullptr || font48 == nullptr)
  {
    std::cout << "failed to load font: " << TTF_GetError() << std::endl;
  }

  initGameWindow();

  frameBuffer = std::make_unique<uint32_t[]>(WIDTH * HEIGHT);
  if (!frameBuffer)
  {
    std::cout << "error allocating frame buffer" << std::endl;
  }

  SpriteFactory::buffInsertInterface(frameBuffer.get(), WIDTH, WIDTH * HEIGHT);

  sprites = SpriteFactory::createSprites();
}

Renderer::~Renderer()
{
  SDL_DestroyTexture(gameTexture);
  SDL_DestroyRenderer(gameRenderer);
  SDL_DestroyWindow(gameWindow);
  SDL_DestroyWindow(configWindow);

  TTF_CloseFont(font24);
  TTF_CloseFont(font48);
  TTF_Quit();

  SDL_Quit();
}

void Renderer::initGameWindow()
{
  gameWindow = SDL_CreateWindow(
      "Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (!gameWindow)
  {
    std::cout << "error creating window: " << SDL_GetError() << std::endl;
  }

  gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!gameRenderer)
  {
    std::cout << "error getting renderer: " << SDL_GetError() << std::endl;
  }

  gameTexture = SDL_CreateTexture(gameRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
  if (!gameTexture)
  {
    std::cout << "error getting texture: " << SDL_GetError() << std::endl;
  }

  gameWindowID = SDL_GetWindowID(gameWindow);
}

void Renderer::initConfigWindow()
{
  configWindow = SDL_CreateWindow(
      "Minesweeper Settings",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      CONFIG_WIDTH,
      CONFIG_HEIGHT,
      SDL_WINDOW_SHOWN);
  if (configWindow == nullptr)
  {
    std::cout << "error creating window: " << SDL_GetError() << std::endl;
  }

  configRenderer = SDL_CreateRenderer(configWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (configRenderer == nullptr)
  {
    std::cout << "error getting config renderer: " << SDL_GetError() << std::endl;
  }

  configWindowID = SDL_GetWindowID(configWindow);
}

void Renderer::renderFrame()
{
  void *pixels;
  int pitch;

  SDL_LockTexture(gameTexture, nullptr, &pixels, &pitch);
  std::memcpy(pixels, frameBuffer.get(), WIDTH * HEIGHT * sizeof(uint32_t));

  SDL_UnlockTexture(gameTexture);
  SDL_RenderCopy(gameRenderer, gameTexture, nullptr, nullptr);
  SDL_RenderPresent(gameRenderer);
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

  // config button
  const auto configButtonSprite = game.getIsConfigButtonPressed() ? sprites->pressedButton : sprites->raisedButton;
  SpriteFactory::copySprite(
      frameBuffer,
      configButtonSprite,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::CONFIG_BUTTON_X,
      config::CONFIG_BUTTON_Y);

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

void Renderer::updateConfigWindow(Minesweeper &game)
{
  if (showConfigWindow != game.getShowConfigButton())
  {
    showConfigWindow = game.getShowConfigButton();
    if (showConfigWindow)
    {
      if (configWindow == nullptr)
      {
        initConfigWindow();
      }

      SDL_SetRenderDrawColor(configRenderer, 255, 255, 255, 255);
      SDL_RenderClear(configRenderer);

      SDL_Surface *textSurface = TTF_RenderText_Solid(font24, "hello", {255, 0, 0, 255});
      if (textSurface == nullptr)
      {
        std::cout << "error creating text surface: " << TTF_GetError() << std::endl;
        return;
      }

      SDL_Texture *textTexture = SDL_CreateTextureFromSurface(configRenderer, textSurface);
      if (textTexture == nullptr)
      {
        std::cout << "error creating text texture: " << TTF_GetError() << std::endl;
        return;
      }

      SDL_Rect textRect = {0, 0, textSurface->w, textSurface->h};
      SDL_RenderCopy(configRenderer, textTexture, nullptr, &textRect);

      SDL_RenderPresent(configRenderer);
      SDL_ShowWindow(configWindow);

      SDL_DestroyTexture(textTexture);
      SDL_FreeSurface(textSurface);
    }
    else
    {
      SDL_HideWindow(configWindow);
    }
  }
}