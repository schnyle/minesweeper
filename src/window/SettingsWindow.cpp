#include <Minesweeper.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SettingsWindow.hpp>
#include <cstdint>
#include <iostream>

#include "font.h"

SettingsWindow::SettingsWindow()
{
  if (TTF_Init() < 0)
  {
    std::cerr << "error initializing SDL TTF: " << TTF_GetError() << std::endl;
  }

  SDL_RWops *rw = SDL_RWFromMem(assets_UbuntuMono_B_ttf, assets_UbuntuMono_B_ttf_len);
  font24 = TTF_OpenFontRW(rw, 0, 24);
  font48 = TTF_OpenFontRW(rw, 0, 48);
  if (font24 == nullptr || font48 == nullptr)
  {
    std::cout << "failed to load font: " << TTF_GetError() << std::endl;
  }
}

void SettingsWindow::init()
{
  WIDTH = config::CONFIG_PIXEL_WIDTH;
  HEIGHT = config::CONFIG_PIXEL_HEIGHT;

  window = SDL_CreateWindow(
      "Minesweeper Settings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (window == nullptr)
  {
    std::cerr << "error creating window: " << SDL_GetError() << std::endl;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr)
  {
    std::cerr << "error getting config renderer: " << SDL_GetError() << std::endl;
  }

  windowID = SDL_GetWindowID(window);
}

void SettingsWindow::update(Minesweeper &game)
{
  if (showConfigWindow != game.getShowConfigButton())
  {
    showConfigWindow = game.getShowConfigButton();
    if (showConfigWindow)
    {
      if (window == nullptr)
      {
        init();
      }

      if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) < 0)
      {
        std::cerr << "error setting render draw color: " << SDL_GetError() << std::endl;
      };

      if (SDL_RenderClear(renderer) < 0)
      {
        std::cerr << "error calling RenderClear: " << SDL_GetError() << std::endl;
      };

      SDL_Surface *textSurface = TTF_RenderText_Solid(font24, "hello", {255, 0, 0, 255});
      if (textSurface == nullptr)
      {
        std::cerr << "error creating text surface: " << TTF_GetError() << std::endl;
        return;
      }

      if (renderer == nullptr)
      {
        printf("fdjakl\n");
      }
      SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
      if (textTexture == nullptr)
      {
        std::cerr << "error creating text texture: " << TTF_GetError() << std::endl;
        return;
      }

      SDL_Rect textRect = {0, 0, textSurface->w, textSurface->h};
      SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

      SDL_RenderPresent(renderer);
      SDL_ShowWindow(window);

      SDL_DestroyTexture(textTexture);
      SDL_FreeSurface(textSurface);
    }
    else
    {
      SDL_HideWindow(window);
    }
  }
}