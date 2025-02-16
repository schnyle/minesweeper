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
  Renderer()
  {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
      std::cerr << "error initializing SDL: " << SDL_GetError() << std::endl;
    }

    gameWindow.init();
  };
  ~Renderer()
  {
    SDL_Quit();
    TTF_Quit();
  };

private:
  GameWindow gameWindow;
  SettingsWindow settingsWindow;
};