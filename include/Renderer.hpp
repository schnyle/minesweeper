#pragma once

#include <Game.hpp>
#include <SpriteFactory.hpp>
#include <X11/Xlib.h>
#include <config.hpp>
#include <cstdint>
#include <map>
#include <memory>
#include <utility>

class Renderer
{
public:
  Renderer();
  ~Renderer();

  void run(Game &);

private:
  Display *display;
  Window root;
  Window window;
  Visual *visual;
  GC gc;
  int screen;
  XImage *image;
  std::unique_ptr<uint32_t[]> frontBuffer;
  std::unique_ptr<uint32_t[]> backBuffer;

  std::unique_ptr<SpriteFactory::Sprites> sprites;

  void renderFrame();
  bool updateGameState(Game &, XEvent &);
  void updateBackBuffer(Game &);

  void initializeGC();
  void initializeBuffers();
};