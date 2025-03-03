#pragma once

#include <Minesweeper.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <config.hpp>
#include <iostream>
#include <memory>

#include "Window/GameWindow.hpp"
#include "Window/SettingsWindow.hpp"

class Renderer
{
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

    config::getSettings().initialize(dm.w, dm.h);

    return true;
  }

  Renderer() { gameWindow.init(); }

  ~Renderer() { SDL_Quit(); };

  GameWindow &getGameWindow() { return gameWindow; };
  SettingsWindow &getSettingsWindow() { return settingsWindow; };

private:
  GameWindow gameWindow;
  SettingsWindow settingsWindow;
};