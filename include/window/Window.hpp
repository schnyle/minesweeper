#pragma once

#include <Minesweeper.hpp>
#include <SDL2/SDL.h>
#include <SpriteFactory.hpp>
#include <cstdint>
#include <memory>
#include <vector>

class Window
{
public:
  Window() { sprites = SpriteFactory::createSprites(); }
  ~Window()
  {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  }

  virtual void init() = 0;
  virtual void update(Minesweeper &) = 0;

  SDL_Window *getWindow() const { return window; }
  SDL_Renderer *getRenderer() const { return renderer; }
  SDL_Texture *getTexture() const { return texture; }
  Uint32 getWindowID() const { return windowID; }

protected:
  int WIDTH;
  int HEIGHT;

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *texture = nullptr;
  Uint32 windowID;

  std::vector<uint32_t> frameBuffer;
  std::unique_ptr<SpriteFactory::Sprites> sprites;
};
