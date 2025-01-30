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

  frameBuffer = std::make_unique<uint32_t[]>(WIDTH * HEIGHT);
  if (!frameBuffer)
  {
    std::cout << "error allocating frame buffer" << std::endl;
  }

  SpriteFactory::buffInsertInterface(frameBuffer.get(), WIDTH, WIDTH * HEIGHT);

  sprites = SpriteFactory::createSprites();

  gameWindow.init(frameBuffer.get());
}

Renderer::~Renderer()
{
  SDL_DestroyWindow(configWindow);

  TTF_CloseFont(font24);
  TTF_CloseFont(font48);
  TTF_Quit();

  SDL_Quit();
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

  SDL_LockTexture(gameWindow.getTexture(), nullptr, &pixels, &pitch);
  std::memcpy(pixels, frameBuffer.get(), WIDTH * HEIGHT * sizeof(uint32_t));

  SDL_UnlockTexture(gameWindow.getTexture());
  SDL_RenderCopy(gameWindow.getRenderer(), gameWindow.getTexture(), nullptr, nullptr);
  SDL_RenderPresent(gameWindow.getRenderer());
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