#pragma once

#include <Minesweeper.hpp>
#include <SDL.h>
#include <SpriteFactory.hpp>
#include <config.hpp>
#include <memory>

class Renderer
{
  friend class GameLoop;

public:
  Renderer();
  ~Renderer();

private:
  static const int WIDTH = config::WINDOW_PIXEL_WIDTH;
  static const int HEIGHT = config::WINDOW_PIXEL_HEIGHT;
  static const int CONFIG_WIDTH = config::CONFIG_PIXEL_WIDTH;
  static const int CONFIG_HEIGHT = config::CONFIG_PIXEL_HEIGHT;

  bool showConfigWindow = false;

  std::unique_ptr<SpriteFactory::Sprites> sprites;
  std::unique_ptr<uint32_t[]> frameBuffer;

  Uint32 gameWindowID;
  SDL_Window *gameWindow = nullptr;
  SDL_Renderer *gameRenderer = nullptr;
  SDL_Texture *gameTexture = nullptr;

  Uint32 configWindowID;
  SDL_Window *configWindow = nullptr;
  SDL_Renderer *configRenderer = nullptr;
  SDL_Texture *configTexture = nullptr;
  SDL_Surface *configSurface = nullptr;

  void initGameWindow();
  void initConfigWindow();
  void renderFrame();
  void updateInterface(Minesweeper &);
  void updateGameArea(Minesweeper &);
  void updateConfigWindow(Minesweeper &);
};