#pragma once

#include <Game.hpp>
#include <X11/Xlib.h>
#include <memory>

class RasterRenderer
{
public:
  RasterRenderer();
  ~RasterRenderer();

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

  void initializeGC();
  void initializeBuffers();
  void renderFrame();

  void updateBackBuffer();

  int rowColToIndex(const int row, const int col) const;
};