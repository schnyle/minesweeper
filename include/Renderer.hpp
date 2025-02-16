#pragma once

#include <Minesweeper.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SpriteFactory.hpp>
#include <config.hpp>
#include <iostream>
#include <memory>

#include "window/GameWindow.hpp"
#include "window/SettingsWindow.hpp"

class Renderer
{
  friend class GameLoop;

public:
  static bool initSDL()
  {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
      std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
      return false;
    }

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    config::DISPLAY_PIXEL_WIDTH = dm.w;
    config::DISPLAY_PIXEL_HEIGHT = dm.h;
    config::update();

    return true;
  }

  Renderer() { gameWindow.init(); };

  ~Renderer()
  {
    SDL_Quit();
    TTF_Quit();
  };

private:
  GameWindow gameWindow;
  SettingsWindow settingsWindow;
};