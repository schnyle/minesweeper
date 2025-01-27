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

  bool isResetButtonPressed = false;

  std::unique_ptr<SpriteFactory::Sprites> sprites;
  std::unique_ptr<uint32_t[]> frameBuffer;

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;

  void initSDL();
  void renderFrame();
  bool updateGameState(Minesweeper &, SDL_Event &);
  void updateInterface(Minesweeper &);
  void updateGameArea(Minesweeper &);
};