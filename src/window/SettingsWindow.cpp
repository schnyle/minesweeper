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

SettingsWindow::~SettingsWindow()
{
  if (font24 != nullptr)
  {
    TTF_CloseFont(font24);
  }
  if (font48 != nullptr)
  {
    TTF_CloseFont(font48);
  }

  TTF_Quit();
}

void SettingsWindow::init()
{
  WIDTH = config::CONFIG_WINDOW_PIXEL_WIDTH;
  HEIGHT = config::CONFIG_WINDOW_PIXEL_HEIGHT;

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
  bool newShowConfig = game.getShowConfigButton();

  if (showConfigWindow != newShowConfig)
  {
    showConfigWindow = newShowConfig;
    if (showConfigWindow)
    {
      if (window == nullptr)
      {
        init();
      }

      SDL_ShowWindow(window);
      renderContent();
    }
    else
    {
      SDL_HideWindow(window);
    }
  }
  else if (showConfigWindow)
  {
    renderContent();
  }
}

void SettingsWindow::handleEvents(SDL_Event &event)
{
  switch (event.type)
  {
  case SDL_TEXTINPUT:
    setting += event.text.text;
    break;
  case SDL_KEYDOWN:
    const auto keycode = event.key.keysym.sym;
    if (keycode == SDLK_BACKSPACE)
    {
      setting = setting.substr(0, setting.size() - 1);
    }
    break;
  }
};

void SettingsWindow::renderContent()
{
  if (SDL_SetRenderDrawColor(renderer, colors.grey.r, colors.grey.g, colors.grey.b, colors.grey.a) < 0)
  {
    std::cerr << "error setting render draw color: " << SDL_GetError() << std::endl;
    return;
  }

  if (SDL_RenderClear(renderer) < 0)
  {
    std::cerr << "error calling RenderClear: " << SDL_GetError() << std::endl;
    return;
  }

  renderText(setting, colors.black, 0, 0);
  renderText("Reset", colors.black, 0, 50);
}

void SettingsWindow::renderText(const std::string text, const SDL_Color &rgba, const int x, const int y)
{
  SDL_Surface *textSurface = TTF_RenderText_Solid(font24, text.c_str(), {rgba.r, rgba.g, rgba.b, rgba.a});
  if (textSurface == nullptr)
  {
    std::cerr << "error creating text surface: " << TTF_GetError() << std::endl;
    return;
  }

  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  if (textTexture == nullptr)
  {
    std::cerr << "error creating text texture: " << SDL_GetError() << std::endl;
    SDL_FreeSurface(textSurface);
    return;
  }

  SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
  SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

  SDL_RenderPresent(renderer);

  SDL_DestroyTexture(textTexture);
  SDL_FreeSurface(textSurface);
};

SDL_Color SettingsWindow::hexToRgba(const uint32_t &hexColor)
{
  SDL_Color color;
  color.r = (hexColor >> 24) & 0xff;
  color.g = (hexColor >> 16) & 0xff;
  color.b = (hexColor >> 8) & 0xff;
  color.a = (hexColor >> 0) & 0xff;

  return color;
}