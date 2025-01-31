#pragma once

#include <Minesweeper.hpp>
#include <SDL.h>
#include <SDL_ttf.h>
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
  static const int WIDTH = config::WINDOW_PIXEL_WIDTH;
  static const int HEIGHT = config::WINDOW_PIXEL_HEIGHT;
  static const int CONFIG_WIDTH = config::CONFIG_PIXEL_WIDTH;
  static const int CONFIG_HEIGHT = config::CONFIG_PIXEL_HEIGHT;

  bool showConfigWindow = false;

  std::unique_ptr<SpriteFactory::Sprites> sprites;
  std::unique_ptr<uint32_t[]> frameBuffer;

  GameWindow gameWindow;
  SettingsWindow settingsWindow;

  Uint32 configWindowID;
  SDL_Window *configWindow = nullptr;
  SDL_Renderer *configRenderer = nullptr;
  SDL_Texture *configTexture = nullptr;
  SDL_Surface *configSurface = nullptr;

  TTF_Font *font24;
  TTF_Font *font48;

  void renderFrame();
};