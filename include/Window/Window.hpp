#pragma once

#include <Minesweeper.hpp>
#include <SDL2/SDL.h>
#include <cstdint>
#include <memory>
#include <vector>

class Window
{
public:
  Window() = default;
  virtual ~Window() = default;

  virtual void init() = 0;
  virtual void update(Minesweeper &) = 0;

  SDL_Window *getWindow() const { return window.get(); }
  SDL_Renderer *getRenderer() const { return renderer.get(); }
  SDL_Texture *getTexture() const { return texture.get(); }
  Uint32 getWindowID() const { return windowID; }

protected:
  int pixelWidth;
  int pixelHeight;

  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{nullptr, &SDL_DestroyWindow};
  std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer{nullptr, &SDL_DestroyRenderer};
  std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture{nullptr, &SDL_DestroyTexture};
  Uint32 windowID;

  std::vector<uint32_t> frameBuffer = {};
};
