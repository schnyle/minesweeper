#pragma once

#include <Minesweeper.hpp>
#include <SDL.h>
#include <SpriteFactory.hpp>
#include <cstdint>
#include <memory>

class Window
{
public:
  Window() { sprites = SpriteFactory::createSprites(); }
  Window(uint32_t *fb) : frameBuffer(fb) { sprites = SpriteFactory::createSprites(); };

  virtual void init(uint32_t *frameBuffer) = 0;
  virtual void updateFrameBuffer(Minesweeper &) = 0;

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

  uint32_t *frameBuffer;
  std::unique_ptr<SpriteFactory::Sprites> sprites;
};
